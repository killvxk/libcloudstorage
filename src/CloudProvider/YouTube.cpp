/*****************************************************************************
 * YouTube.cpp : YouTube implementation
 *
 *****************************************************************************
 * Copyright (C) 2016-2016 VideoLAN
 *
 * Authors: Paweł Wegner <pawel.wegner95@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#include "YouTube.h"

#include <jsoncpp/json/json.h>

#include "Utility/Item.h"
#include "Utility/Utility.h"

namespace cloudstorage {

YouTube::YouTube() : CloudProvider(make_unique<Auth>()) {}

std::string YouTube::name() const { return "youtube"; }

HttpRequest::Pointer YouTube::getItemDataRequest(const std::string& id,
                                                 std::ostream&) const {
  auto request = make_unique<HttpRequest>(
      "https://www.googleapis.com/youtube/v3/playlistItems",
      HttpRequest::Type::GET);
  request->setParameter("part", "contentDetails,snippet");
  request->setParameter("id", id);
  return request;
}

HttpRequest::Pointer YouTube::listDirectoryRequest(
    const IItem& item, const std::string& page_token, std::ostream&) const {
  if (item.id() == rootDirectory()->id()) {
    if (page_token.empty())
      return make_unique<HttpRequest>(
          "https://www.googleapis.com/youtube/v3/"
          "channels?mine=true&part=contentDetails,snippet",
          HttpRequest::Type::GET);
    else if (page_token == "real_playlist")
      return make_unique<HttpRequest>(
          "https://www.googleapis.com/youtube/v3/"
          "playlists?mine=true&part=snippet",
          HttpRequest::Type::GET);
    else
      return make_unique<HttpRequest>(
          "https://www.googleapis.com/youtube/v3/"
          "playlists?mine=true&part=snippet&pageToken=" +
              page_token,
          HttpRequest::Type::GET);
  } else {
    auto request = make_unique<HttpRequest>(
        "https://www.googleapis.com/youtube/v3/playlistItems",
        HttpRequest::Type::GET);
    request->setParameter("part", "snippet");
    request->setParameter("playlistId", item.id());
    if (!page_token.empty()) request->setParameter("pageToken", page_token);
    return request;
  }
}

HttpRequest::Pointer YouTube::uploadFileRequest(const IItem&,
                                                const std::string&,
                                                std::istream&,
                                                std::ostream&) const {
  return nullptr;
}

HttpRequest::Pointer YouTube::downloadFileRequest(const IItem& item,
                                                  std::ostream&) const {
  return make_unique<HttpRequest>(item.url(), HttpRequest::Type::GET);
}

IItem::Pointer YouTube::getItemDataResponse(std::istream& stream) const {
  Json::Value response;
  stream >> response;
  return toItem(response["items"][0], response["kind"].asString());
}

std::vector<IItem::Pointer> YouTube::listDirectoryResponse(
    std::istream& stream, std::string& next_page_token) const {
  Json::Value response;
  stream >> response;
  std::vector<IItem::Pointer> result;
  if (response["kind"].asString() == "youtube#channelListResponse") {
    Json::Value related_playlits =
        response["items"][0]["contentDetails"]["relatedPlaylists"];
    for (const std::string& name : related_playlits.getMemberNames()) {
      auto item = make_unique<Item>(name, related_playlits[name].asString(),
                                    IItem::FileType::Directory);

      item->set_thumbnail_url(response["items"][0]["snippet"]["thumbnails"]
                                      ["default"]["url"]
                                          .asString());
      result.push_back(std::move(item));
    }
    next_page_token = "real_playlist";
  } else {
    for (const Json::Value& v : response["items"])
      result.push_back(toItem(v, response["kind"].asString()));
  }

  if (response.isMember("nextPageToken"))
    next_page_token = response["nextPageToken"].asString();
  return result;
}

IItem::Pointer YouTube::toItem(const Json::Value& v, std::string kind) const {
  if (kind == "youtube#playlistListResponse") {
    auto item =
        make_unique<Item>(v["snippet"]["title"].asString(), v["id"].asString(),
                          IItem::FileType::Directory);
    item->set_thumbnail_url(
        v["snippet"]["thumbnails"]["default"]["url"].asString());
    return item;
  } else {
    auto item = make_unique<Item>(v["snippet"]["title"].asString(),
                                  v["id"].asString(), IItem::FileType::Video);
    item->set_thumbnail_url(
        v["snippet"]["thumbnails"]["default"]["url"].asString());
    item->set_url(
        "http://youtube-dl.appspot.com/api/play?url=https://www.youtube.com/"
        "watch?v=" +
        v["snippet"]["resourceId"]["videoId"].asString());
    return item;
  }
}

std::string YouTube::Auth::authorizeLibraryUrl() const {
  return "https://accounts.google.com/o/oauth2/auth?client_id=" + client_id() +
         "&redirect_uri=" + redirect_uri() +
         "&scope=https://www.googleapis.com/auth/"
         "youtube&response_type=code&access_type=offline";
}

}  // namespace cloudstorage

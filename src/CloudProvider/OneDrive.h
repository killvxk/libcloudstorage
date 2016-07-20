/*****************************************************************************
 * OneDrive.h : prototypes for OneDrive
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

#ifndef ONEDRIVE_H
#define ONEDRIVE_H

#include "CloudProvider.h"
#include "Utility/Auth.h"

namespace cloudstorage {

class OneDrive : public CloudProvider {
 public:
  OneDrive();

  std::string name() const;

 protected:
  HttpRequest::Pointer getItemDataRequest(const std::string&,
                                          std::ostream& input_stream) const;
  HttpRequest::Pointer listDirectoryRequest(const IItem&,
                                            const std::string& page_token,
                                            std::ostream& input_stream) const;
  HttpRequest::Pointer uploadFileRequest(const IItem& directory,
                                         const std::string& filename,
                                         std::istream& stream,
                                         std::ostream& input_stream) const;
  HttpRequest::Pointer downloadFileRequest(const IItem&,
                                           std::ostream& input_stream) const;
  HttpRequest::Pointer deleteItemRequest(const IItem&,
                                         std::ostream& input_stream) const;

  std::vector<IItem::Pointer> listDirectoryResponse(std::istream&,
                                                    std::string&) const;
  IItem::Pointer getItemDataResponse(std::istream& response) const;

 private:
  class Auth : public cloudstorage::Auth {
   public:
    Auth();

    std::string authorizeLibraryUrl() const;

    HttpRequest::Pointer exchangeAuthorizationCodeRequest(
        std::ostream& input_data) const;
    HttpRequest::Pointer refreshTokenRequest(std::ostream& input_data) const;

    Token::Pointer exchangeAuthorizationCodeResponse(std::istream&) const;
    Token::Pointer refreshTokenResponse(std::istream&) const;
  };

  IItem::Pointer toItem(const Json::Value&) const;
};

}  // namespace cloudstorage

#endif  // ONEDRIVE_H

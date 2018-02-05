#include "CloudItem.h"

#include "Request/ListDirectory.h"

#undef CreateDirectory

using namespace cloudstorage;

CloudItem::CloudItem(const Provider& p, IItem::Pointer item, QObject* parent)
    : QObject(parent), provider_(p), item_(item) {}

QString CloudItem::filename() const { return item_->filename().c_str(); }

QString CloudItem::type() const {
  switch (item_->type()) {
    case IItem::FileType::Unknown:
      return "file";
    case IItem::FileType::Audio:
      return "audio";
    case IItem::FileType::Image:
      return "image";
    case IItem::FileType::Video:
      return "video";
    case IItem::FileType::Directory:
      return "directory";
    default:
      return "";
  }
}

bool CloudItem::supports(QString operation) const {
  auto provider = this->provider().provider_;
  if (operation == "delete")
    return provider->supportedOperations() & ICloudProvider::DeleteItem;
  else if (operation == "upload")
    return provider->supportedOperations() & ICloudProvider::UploadFile;
  else if (operation == "move")
    return provider->supportedOperations() & ICloudProvider::MoveItem;
  else if (operation == "rename")
    return provider->supportedOperations() & ICloudProvider::RenameItem;
  else if (operation == "mkdir")
    return provider->supportedOperations() & ICloudProvider::CreateDirectory;
  else if (operation == "download")
    return provider->supportedOperations() & ICloudProvider::DownloadFile;
  else
    return true;
}

ListDirectoryCacheKey CloudItem::key() const {
  return {provider().provider_->name(), provider().label_, item()->id()};
}

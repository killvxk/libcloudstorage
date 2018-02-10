#ifndef DESKTOPUTILITY_H
#define DESKTOPUTILITY_H

#include "WinRTUtility.h"

#ifndef __ANDROID__
#ifndef WINRT

#define USE_DESKTOP_UTILITY

#include "IPlatformUtility.h"

class DesktopUtility : public IPlatformUtility {
 public:
  bool mobile() const override;
  QString name() const override;
  bool openWebPage(QString url) override;
  void closeWebPage() override;
  void landscapeOrientation() override;
  void defaultOrientation() override;
  void showPlayerNotification(bool playing, QString filename,
                              QString title) override;
  void hidePlayerNotification() override;
  void enableKeepScreenOn() override;
  void disableKeepScreenOn() override;
  void showAd() override;
  void hideAd() override;
};

#endif  // WINRT
#endif  // __ANDROID__

#endif  // DESKTOPUTILITY_H
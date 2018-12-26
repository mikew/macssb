@import WebKit;

/*
 * Copyright (C) 2014-2016 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

//#include "WKPreferencesRefPrivate.h"

#ifdef __OBJC__

#import <WebKit/WKPreferences.h>

#if WK_API_ENABLED

typedef NS_ENUM(NSInteger, _WKStorageBlockingPolicy) {
    _WKStorageBlockingPolicyAllowAll,
    _WKStorageBlockingPolicyBlockThirdParty,
    _WKStorageBlockingPolicyBlockAll,
};

typedef NS_OPTIONS(NSUInteger, _WKDebugOverlayRegions) {
    _WKNonFastScrollableRegion = 1 << 0,
    _WKWheelEventHandlerRegion = 1 << 1
};

typedef NS_OPTIONS(NSUInteger, _WKJavaScriptRuntimeFlags) {
    _WKJavaScriptRuntimeFlagsAllEnabled = 0
};

typedef NS_ENUM(NSInteger, _WKEditableLinkBehavior) {
    _WKEditableLinkBehaviorDefault,
    _WKEditableLinkBehaviorAlwaysLive,
    _WKEditableLinkBehaviorOnlyLiveWithShiftKey,
    _WKEditableLinkBehaviorLiveWhenNotFocused,
    _WKEditableLinkBehaviorNeverLive,
};

@class _WKExperimentalFeature;

@interface WKPreferences () <NSCopying>
@end

@interface WKPreferences (WKPrivate)

// FIXME: This property should not have the verb "is" in it.
@property (nonatomic, setter=_setTelephoneNumberDetectionIsEnabled:) BOOL _telephoneNumberDetectionIsEnabled;
@property (nonatomic, setter=_setStorageBlockingPolicy:) _WKStorageBlockingPolicy _storageBlockingPolicy;

@property (nonatomic, setter=_setCompositingBordersVisible:) BOOL _compositingBordersVisible;
@property (nonatomic, setter=_setCompositingRepaintCountersVisible:) BOOL _compositingRepaintCountersVisible;
@property (nonatomic, setter=_setTiledScrollingIndicatorVisible:) BOOL _tiledScrollingIndicatorVisible;
@property (nonatomic, setter=_setResourceUsageOverlayVisible:) BOOL _resourceUsageOverlayVisible;
@property (nonatomic, setter=_setVisibleDebugOverlayRegions:) _WKDebugOverlayRegions _visibleDebugOverlayRegions;
@property (nonatomic, setter=_setSimpleLineLayoutEnabled:) BOOL _simpleLineLayoutEnabled;
@property (nonatomic, setter=_setSimpleLineLayoutDebugBordersEnabled:) BOOL _simpleLineLayoutDebugBordersEnabled;
@property (nonatomic, setter=_setAcceleratedDrawingEnabled:) BOOL _acceleratedDrawingEnabled;
@property (nonatomic, setter=_setDisplayListDrawingEnabled:) BOOL _displayListDrawingEnabled;
@property (nonatomic, setter=_setVisualViewportEnabled:) BOOL _visualViewportEnabled;
@property (nonatomic, setter=_setLargeImageAsyncDecodingEnabled:) BOOL _largeImageAsyncDecodingEnabled;
@property (nonatomic, setter=_setAnimatedImageAsyncDecodingEnabled:) BOOL _animatedImageAsyncDecodingEnabled;
@property (nonatomic, setter=_setTextAutosizingEnabled:) BOOL _textAutosizingEnabled;
@property (nonatomic, setter=_setSubpixelAntialiasedLayerTextEnabled:) BOOL _subpixelAntialiasedLayerTextEnabled;

@property (nonatomic, setter=_setDeveloperExtrasEnabled:) BOOL _developerExtrasEnabled;

@property (nonatomic, setter=_setLogsPageMessagesToSystemConsoleEnabled:) BOOL _logsPageMessagesToSystemConsoleEnabled;

@property (nonatomic, setter=_setHiddenPageDOMTimerThrottlingEnabled:) BOOL _hiddenPageDOMTimerThrottlingEnabled;
@property (nonatomic, setter=_setHiddenPageDOMTimerThrottlingAutoIncreases:) BOOL _hiddenPageDOMTimerThrottlingAutoIncreases;
@property (nonatomic, setter=_setPageVisibilityBasedProcessSuppressionEnabled:) BOOL _pageVisibilityBasedProcessSuppressionEnabled;

@property (nonatomic, setter=_setAllowFileAccessFromFileURLs:) BOOL _allowFileAccessFromFileURLs;
@property (nonatomic, setter=_setJavaScriptRuntimeFlags:) _WKJavaScriptRuntimeFlags _javaScriptRuntimeFlags;

@property (nonatomic, setter=_setStandalone:, getter=_isStandalone) BOOL _standalone;

@property (nonatomic, setter=_setDiagnosticLoggingEnabled:) BOOL _diagnosticLoggingEnabled;

@property (nonatomic, setter=_setDefaultFontSize:) NSUInteger _defaultFontSize;
@property (nonatomic, setter=_setDefaultFixedPitchFontSize:) NSUInteger _defaultFixedPitchFontSize;
@property (nonatomic, copy, setter=_setFixedPitchFontFamily:) NSString *_fixedPitchFontFamily;

// FIXME: This should be configured on the WKWebsiteDataStore.
// FIXME: This property should not have the verb "is" in it.
@property (nonatomic, setter=_setOfflineApplicationCacheIsEnabled:) BOOL _offlineApplicationCacheIsEnabled;
@property (nonatomic, setter=_setFullScreenEnabled:) BOOL _fullScreenEnabled;
@property (nonatomic, setter=_setShouldSuppressKeyboardInputDuringProvisionalNavigation:) BOOL _shouldSuppressKeyboardInputDuringProvisionalNavigation;
@property (nonatomic, setter=_setAllowsPictureInPictureMediaPlayback:) BOOL _allowsPictureInPictureMediaPlayback;

@property (nonatomic, setter=_setApplePayCapabilityDisclosureAllowed:) BOOL _applePayCapabilityDisclosureAllowed;

@property (nonatomic, setter=_setLoadsImagesAutomatically:) BOOL _loadsImagesAutomatically;

@property (nonatomic, setter=_setPeerConnectionEnabled:) BOOL _peerConnectionEnabled;
@property (nonatomic, setter=_setMediaDevicesEnabled:) BOOL _mediaDevicesEnabled;
@property (nonatomic, setter=_setScreenCaptureEnabled:) BOOL _screenCaptureEnabled;
@property (nonatomic, setter=_setMockCaptureDevicesEnabled:) BOOL _mockCaptureDevicesEnabled;
@property (nonatomic, setter=_setMockCaptureDevicesPromptEnabled:) BOOL _mockCaptureDevicesPromptEnabled;
@property (nonatomic, setter=_setMediaCaptureRequiresSecureConnection:) BOOL _mediaCaptureRequiresSecureConnection;
@property (nonatomic, setter=_setEnumeratingAllNetworkInterfacesEnabled:) BOOL _enumeratingAllNetworkInterfacesEnabled;
@property (nonatomic, setter=_setICECandidateFilteringEnabled:) BOOL _iceCandidateFilteringEnabled;
@property (nonatomic, setter=_setWebRTCLegacyAPIEnabled:) BOOL _webRTCLegacyAPIEnabled;
@property (nonatomic, setter=_setInactiveMediaCaptureSteamRepromptIntervalInMinutes:) double _inactiveMediaCaptureSteamRepromptIntervalInMinutes;

@property (nonatomic, setter=_setJavaScriptCanAccessClipboard:) BOOL _javaScriptCanAccessClipboard;
@property (nonatomic, setter=_setDOMPasteAllowed:) BOOL _domPasteAllowed;

@property (nonatomic, setter=_setShouldAllowUserInstalledFonts:) BOOL _shouldAllowUserInstalledFonts;

@property (nonatomic, setter=_setEditableLinkBehavior:) _WKEditableLinkBehavior _editableLinkBehavior;

+ (NSArray<_WKExperimentalFeature *> *)_experimentalFeatures;
- (BOOL)_isEnabledForFeature:(_WKExperimentalFeature *)feature;
- (void)_setEnabled:(BOOL)value forFeature:(_WKExperimentalFeature *)feature;

//#if !TARGET_OS_IPHONE
@property (nonatomic, setter=_setWebGLEnabled:) BOOL _webGLEnabled;
@property (nonatomic, setter=_setJavaEnabledForLocalFiles:) BOOL _javaEnabledForLocalFiles;
@property (nonatomic, setter=_setCanvasUsesAcceleratedDrawing:) BOOL _canvasUsesAcceleratedDrawing;
@property (nonatomic, setter=_setAcceleratedCompositingEnabled:) BOOL _acceleratedCompositingEnabled;
@property (nonatomic, setter=_setDefaultTextEncodingName:) NSString *_defaultTextEncodingName;
@property (nonatomic, setter=_setNeedsSiteSpecificQuirks:) BOOL _needsSiteSpecificQuirks;
@property (nonatomic, setter=_setAuthorAndUserStylesEnabled:) BOOL _authorAndUserStylesEnabled;
@property (nonatomic, setter=_setDOMTimersThrottlingEnabled:) BOOL _domTimersThrottlingEnabled;
@property (nonatomic, setter=_setWebArchiveDebugModeEnabled:) BOOL _webArchiveDebugModeEnabled;
@property (nonatomic, setter=_setLocalFileContentSniffingEnabled:) BOOL _localFileContentSniffingEnabled;
@property (nonatomic, setter=_setUsesPageCache:) BOOL _usesPageCache;
@property (nonatomic, setter=_setPageCacheSupportsPlugins:) BOOL _pageCacheSupportsPlugins;
@property (nonatomic, setter=_setShouldPrintBackgrounds:) BOOL _shouldPrintBackgrounds;
@property (nonatomic, setter=_setWebSecurityEnabled:) BOOL _webSecurityEnabled;
@property (nonatomic, setter=_setUniversalAccessFromFileURLsAllowed:) BOOL _universalAccessFromFileURLsAllowed;
@property (nonatomic, setter=_setAVFoundationEnabled:) BOOL _avFoundationEnabled;
@property (nonatomic, setter=_setSuppressesIncrementalRendering:) BOOL _suppressesIncrementalRendering;
@property (nonatomic, setter=_setAsynchronousPluginInitializationEnabled:) BOOL _asynchronousPluginInitializationEnabled;
@property (nonatomic, setter=_setArtificialPluginInitializationDelayEnabled:) BOOL _artificialPluginInitializationDelayEnabled;
@property (nonatomic, setter=_setCookieEnabled:) BOOL _cookieEnabled;
@property (nonatomic, setter=_setPlugInSnapshottingEnabled:) BOOL _plugInSnapshottingEnabled;
@property (nonatomic, setter=_setSubpixelCSSOMElementMetricsEnabled:) BOOL _subpixelCSSOMElementMetricsEnabled;
@property (nonatomic, setter=_setMediaSourceEnabled:) BOOL _mediaSourceEnabled;
@property (nonatomic, setter=_setViewGestureDebuggingEnabled:) BOOL _viewGestureDebuggingEnabled;
@property (nonatomic, setter=_setCSSAnimationTriggersEnabled:) BOOL _cssAnimationTriggersEnabled;
@property (nonatomic, setter=_setStandardFontFamily:) NSString *_standardFontFamily;
@property (nonatomic, setter=_setNotificationsEnabled:) BOOL _notificationsEnabled;
@property (nonatomic, setter=_setBackspaceKeyNavigationEnabled:) BOOL _backspaceKeyNavigationEnabled;
//#endif

@end

#endif

#endif

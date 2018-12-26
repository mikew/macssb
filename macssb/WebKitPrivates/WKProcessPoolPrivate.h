/*
 * Copyright (C) 2014-2017 Apple Inc. All rights reserved.
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

#import <WebKit/WKProcessPool.h>

#if WK_API_ENABLED

@class _WKAutomationSession;
@class _WKProcessPoolConfiguration;
@protocol _WKAutomationDelegate;
@protocol _WKDownloadDelegate;
@protocol _WKGeolocationCoreLocationProvider;

@interface WKProcessPool ()
- (instancetype)_initWithConfiguration:(_WKProcessPoolConfiguration *)configuration __attribute__((objc_method_family(init))) NS_DESIGNATED_INITIALIZER;
@end

@interface WKProcessPool (WKPrivate)

+ (WKProcessPool *)_sharedProcessPool;

@property (nonatomic, readonly) _WKProcessPoolConfiguration *_configuration;

- (void)_setAllowsSpecificHTTPSCertificate:(NSArray *)certificateChain forHost:(NSString *)host;
- (void)_setCanHandleHTTPSServerTrustEvaluation:(BOOL)value;
- (void)_setCookieAcceptPolicy:(NSHTTPCookieAcceptPolicy)policy;

- (id)_objectForBundleParameter:(NSString *)parameter;
- (void)_setObject:(id <NSCopying, NSSecureCoding>)object forBundleParameter:(NSString *)parameter;
// FIXME: This should be NSDictionary<NSString *, id <NSCopying, NSSecureCoding>>
- (void)_setObjectsForBundleParametersWithDictionary:(NSDictionary *)dictionary;

#if !TARGET_OS_IPHONE
- (void)_resetPluginLoadClientPolicies:(NSDictionary *)policies;
@property (nonatomic, readonly, copy) NSDictionary *_pluginLoadClientPolicies;
#endif

@property (nonatomic, weak, setter=_setDownloadDelegate:) id <_WKDownloadDelegate> _downloadDelegate;
@property (nonatomic, weak, setter=_setAutomationDelegate:) id <_WKAutomationDelegate> _automationDelegate;

#if TARGET_OS_IPHONE
@property (nonatomic, setter=_setCoreLocationProvider:) id <_WKGeolocationCoreLocationProvider> _coreLocationProvider WK_API_AVAILABLE(ios(11.0));
#endif

+ (NSURL *)_websiteDataURLForContainerWithURL:(NSURL *)containerURL;
+ (NSURL *)_websiteDataURLForContainerWithURL:(NSURL *)containerURL bundleIdentifierIfNotInContainer:(NSString *)bundleIdentifier;

- (void)_warmInitialProcess;
- (void)_automationCapabilitiesDidChange;
- (void)_setAutomationSession:(_WKAutomationSession *)automationSession;

// Test only. Should be called only while no web content processes are running.
- (void)_terminateStorageProcess;
- (void)_terminateNetworkProcess;
- (void)_terminateServiceWorkerProcess;

// Test only.
- (pid_t)_networkProcessIdentifier;
- (pid_t)_storageProcessIdentifier;

// Test only.
- (size_t)_webProcessCount;
- (size_t)_pluginProcessCount;
- (void)_syncNetworkProcessCookies;

// Test only. Returns web processes running web pages (does not include web processes running service workers)
- (size_t)_webPageContentProcessCount;

// Test only. Should be called before any web content processes are launched.
+ (void)_forceGameControllerFramework;

- (void)_preconnectToServer:(NSURL *)serverURL;

// Test only.
- (void)_setAllowsAnySSLCertificateForServiceWorker:(BOOL)allows;
- (void)_registerURLSchemeServiceWorkersCanHandle:(NSString *)scheme;
- (void)_setMaximumNumberOfProcesses:(NSUInteger)value;

@property (nonatomic, getter=_isCookieStoragePartitioningEnabled, setter=_setCookieStoragePartitioningEnabled:) BOOL _cookieStoragePartitioningEnabled;
@property (nonatomic, getter=_isStorageAccessAPIEnabled, setter=_setStorageAccessAPIEnabled:) BOOL _storageAccessAPIEnabled;

@end

#endif

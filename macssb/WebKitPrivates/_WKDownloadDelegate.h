/*
 * Copyright (C) 2014 Apple Inc. All rights reserved.
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

#import "WKFoundation.h"

#if WK_API_ENABLED

#import <Foundation/Foundation.h>

@class _WKDownload;

@protocol _WKDownloadDelegate <NSObject>
@optional
- (void)_downloadDidStart:(_WKDownload *)download;
- (void)_download:(_WKDownload *)download didReceiveServerRedirectToURL:(NSURL *)url;
- (void)_download:(_WKDownload *)download didReceiveResponse:(NSURLResponse *)response;
- (void)_download:(_WKDownload *)download didReceiveData:(uint64_t)length;
- (void)_download:(_WKDownload *)download decideDestinationWithSuggestedFilename:(NSString *)filename completionHandler:(void (^)(BOOL allowOverwrite, NSString *destination))completionHandler;
- (void)_downloadDidFinish:(_WKDownload *)download;
- (void)_download:(_WKDownload *)download didFailWithError:(NSError *)error;
- (void)_downloadDidCancel:(_WKDownload *)download;
- (void)_download:(_WKDownload *)download didReceiveAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge completionHandler:(void (^)(NSURLSessionAuthChallengeDisposition, NSURLCredential*))completionHandler;
- (void)_download:(_WKDownload *)download didCreateDestination:(NSString *)destination;
- (void)_downloadProcessDidCrash:(_WKDownload *)download;
- (BOOL)_download:(_WKDownload *)download shouldDecodeSourceDataOfMIMEType:(NSString *)MIMEType;

- (NSString *)_download:(_WKDownload *)download decideDestinationWithSuggestedFilename:(NSString *)filename allowOverwrite:(BOOL *)allowOverwrite;
@end

#endif
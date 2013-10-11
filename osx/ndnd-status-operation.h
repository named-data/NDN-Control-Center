/* -*- Mode: objc; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 * @author Ilya Moiseenko <iliamo@ucla.edu>
 */

#import <Cocoa/Cocoa.h>
#import "menu-delegate.h"

@interface NdndStatusOperation : NSOperation
{
  MenuDelegate *m_delegate;
}

-(id)initWithDelegate:(MenuDelegate*)delegate;

@end

/* -*- Mode: objc; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * @copyright See LICENCE for copyright and license information.
 *
 * @author Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 * @author Ilya Moiseenko <iliamo@ucla.edu>
 */

#import <Cocoa/Cocoa.h>

@interface FibTableController : NSObject <NSTableViewDataSource, NSXMLParserDelegate>
{
  NSXMLDocument *m_document;
}

@property NSTableView *m_tableView;

- (void)loadStatus:(NSXMLDocument *)document;
- (NSString *)getFaceByRowIndex:(NSInteger)index;
- (NSString *)getPrefixByRowIndex:(NSInteger)index;

@end

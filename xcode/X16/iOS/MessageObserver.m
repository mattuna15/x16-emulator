//
//  MessageObserver.m
//  CommanderX16
//
//  Created by Pearce, Matthew (Senior Developer) on 04/01/2020.
//

#import "MessageObserver.h"
#import "SettingsViewController.h"
#import "SDL.h"
#import "SDL_uikitappdelegate.h"

@interface MessageObserver ()
@property (nonatomic, retain) SettingsViewController *settings;
@end

@implementation MessageObserver

@synthesize settings;

-(id)init {
    
    if (self == [super init]) {
        
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(showSettings) name:@"ShowKeyboard" object:nil];
        
    }
    
    return self;
}

-(void)showSettings {
    
    if (!settings) {
        settings = [[SettingsViewController alloc] initWithNibName:@"SettingsViewController" bundle:nil];
    } else {
        [[[self rootView] viewWithTag:666] removeFromSuperview];
    }
    
    CGRect rect = settings.view.frame;
    
    CGPoint origin = CGPointMake(0, 0);
    rect.origin = origin;
    settings.view.frame = rect;
    
    settings.view.tag = 666;
    
    [[self rootView] addSubview:settings.view];
}

-(UIView *)rootView {
    SDLUIKitDelegate *delegate = [SDLUIKitDelegate sharedAppDelegate];
    UIWindow *window = [delegate window];
    return [window rootViewController].view;
}


@end
//
//  Header.h
//  cOS
//
//  Created by yihong dai on 9/8/14.
//  Copyright (c) 2014 yihong dai. All rights reserved.
//

#ifndef cOS_Header_h
#define cOS_Header_h

struct Node {
	char* path;
	struct Node * next;
} ;

struct LinkedList{
	struct Node * head ;
	struct Node * end;
} ;
#endif

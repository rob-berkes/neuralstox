/*
 *  RobsLib.cp
 *  RobsLib
 *
 *  Created by Robert on 3/18/14.
 *  Copyright (c) 2014 ___ROBERTBERKES___. All rights reserved.
 *
 */

#include <iostream>
#include "RobsLib.h"
#include "RobsLibPriv.h"

void RobsLib::HelloWorld(const char * s)
{
	 RobsLibPriv *theObj = new RobsLibPriv;
	 theObj->HelloWorldPriv(s);
	 delete theObj;
};

void RobsLibPriv::HelloWorldPriv(const char * s) 
{
	std::cout << s << std::endl;
};


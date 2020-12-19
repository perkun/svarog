#!/bin/python3
# created by perkun on 19/12/2020

import sys
print('"' + repr( chr( int( sys.argv[1], 16 )).encode( 'utf-8' ))[ 2:-1 ] + '"')


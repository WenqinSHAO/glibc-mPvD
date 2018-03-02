#!/bin/sh

. `dirname $0`/check-nc.sh

{
	echo PVD_CONNECTION_PROMOTE_CONTROL
	echo PVD_BEGIN_TRANSACTION pvd.cisco.com
	echo PVD_SET_ATTRIBUTE pvd.cisco.com rdnss '["2001:4860:4860::8844", "2620:74:1b::1:1"]'
	echo PVD_SET_ATTRIBUTE pvd.cisco.com dnssl '["orange.fr", "proximus.be"]'
	echo PVD_END_TRANSACTION pvd.cisco.com
} | $NC 0.0.0.0 10101

#!/bin/bash
pkill gxf_server
registry cache -c
rm -rf /var/tmp/gxf/default_repo/*
registry repo sync -n ngc-public
registry repo sync -n default

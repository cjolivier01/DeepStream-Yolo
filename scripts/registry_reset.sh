#!/bin/bash
registry cache -c
rm -rf /var/tmp/gxf/default_repo/*
registry repo sync -n ngc-public
registry repo sync -n default

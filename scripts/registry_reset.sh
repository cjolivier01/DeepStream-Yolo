#!/bin/bash
registry cache -c
registry repo sync -n ngc-public
registry repo sync -n default

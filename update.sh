#!/bin/sh
# Rebase current master branch onto custom branch. 
# See: http://dwm.suckless.org/customisation/patches_in_git
git checkout master
git pull
git checkout custom
git rebase --preserve-merges master

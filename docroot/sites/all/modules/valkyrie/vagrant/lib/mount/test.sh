#!/bin/bash
red='\033[00;31m'
green='\033[00;32m'
blue='\033[00;34m'

uid=`id -u`
gid=`id -g`
username=`whoami`
group=`id -gn`

report_result() {
  if (( $? > 0 )); then
    echo -e "${red}ERROR"
    exit 1
  else
    echo -e "${green}OK"
  fi;
  tput sgr0
}

echo 'Bringing up VM. This may take a few minutes, and you may be prompted for your sudo password.'
vagrant up

echo -e "\n ${blue}** BEGINNING TESTS **\n"; tput sgr0

echo 'Checking whether test user exists.'
vagrant ssh -c'getent passwd test_user' -- -q > /dev/null
if (( $? > 0 )); then
  echo 'Test user not found. Creating test user on VM.'
  vagrant ssh -c"sudo adduser test_user -u ${uid} --gid ${gid} --disabled-password --gecos ''" -- -q > /dev/null
  report_result
else
  echo -e "${green}OK"; tput sgr0
fi

echo 'Checking NFS mount on host.'
grep 'nfs_test" 10.234.234.234' /etc/exports > /dev/null; report_result

echo 'Checking NFS mount on VM.'
vagrant ssh -c'mount'  -- -q | grep nfs_test > /dev/null; report_result

echo 'Creating local test file.'
echo 'test1234' > nfs_test/local.txt; report_result

echo 'Checking that local test file exists on VM.'
vagrant ssh -c'[ -f /tmp/nfs_test/local.txt ]'  -- -q > /dev/null; report_result

echo 'Checking local test file contents on VM.'
vagrant ssh -c'cat /tmp/nfs_test/local.txt'  -- -q | grep ^test1234 > /dev/null; report_result

echo 'Checking local test file ownership on VM.'
vagrant ssh -c'ls -la /tmp/nfs_test/local.txt' -- -q | grep "test_user www-data" > /dev/null; report_result

echo 'Creating remote test file.'
vagrant ssh -c'echo test4321 > /tmp/nfs_test/remote.txt' -- -q > /dev/null; report_result

echo 'Checking that remote test file exists locally.'
[ -f nfs_test/remote.txt ]; report_result

echo 'Checking remote test file contents locally.'
cat nfs_test/remote.txt | grep ^test4321 > /dev/null; report_result

echo 'Checking remote test file ownership locally.'
ls -la nfs_test/remote.txt | grep -E "(${username}|${group})" > /dev/null; report_result

echo -e "\n ${green}** ALL TESTS PASSED **\n"; tput sgr0

echo 'Tearing down VM.'
vagrant destroy -f

if (( ! $? > 0 )); then
  echo 'Removing test files.'
  rm nfs_test/*
fi

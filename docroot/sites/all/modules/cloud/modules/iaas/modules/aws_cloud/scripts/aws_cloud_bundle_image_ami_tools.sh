#!/bin/bash

##
# @file
# This script takes care of bundling an S3 backed ami
# 
# Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
#
##

# Curl Options
CURL_OPTS='--silent --retry 1 --retry-delay 1 --retry-max-time 1 --max-time 30'
EC2_AMI_TOOL_URL='http://s3.amazonaws.com/ec2-downloads/ec2-ami-tools.zip'

# Bundle Specific values
IMAGE_NAME='@CLANAVI_IMAGE_NAME'
BUCKET='@CLANAVI_BUCKET_NAME/@CLANAVI_FOLDER_NAME'
SIZE='@CLANAVI_SIZE'
REGION='@CLANAVI_REGION'

# AWS Credentials
AWS_USER_ID='@CLANAVI_AWS_USER_ID'
AWS_ACCESS_KEY_ID='@CLANAVI_AWS_ACCESS_KEY_ID'
AWS_SECRET_ACCESS_KEY='@CLANAVI_AWS_SECRET_ACCESS_KEY'

# Script location
CLANAVI_DIR='/tmp/clanavi'

# EC2 Key Location
EC2_HOME_DIR="$CLANAVI_DIR/.ec2"
EC2_PRIVATE_KEY_FILE="$EC2_HOME_DIR/pk.pem"
EC2_CERT_FILE="$EC2_HOME_DIR/cert.pem"

# Export the variables used by EC2 AMI Tools
export EC2_PRIVATE_KEY=$EC2_PRIVATE_KEY_FILE
export EC2_CERT=$EC2_CERT_FILE
export EC2_HOME='/usr/local/ec2-ami-tools'

# Other Variables TBD
DEBUG=1
TMP_FILE_NAME=`date +%Y-%m-%d-%T`
LOG_FILE="$CLANAVI_DIR/bundle_image-test.log"

# Identify System Architecture
if [ $(uname -m) = 'x86_64' ]; then
  arch=x86_64
else
  arch=i386
fi

# Deterime which package manager to use
if which apt-get >/dev/null; then
  PACKAGE_MANAGEMENT="`which apt-get`"
else
  PACKAGE_MANAGEMENT="`which yum`"
fi

##
# The following are functions to bundle the instance
##

# Update script status
updatestatus() {
  # remove old state before writing new state
  rm $CLANAVI_DIR/bundle_instance.*
  touch $CLANAVI_DIR/bundle_instance.$1
}

# Clean up flags and log files before next run
cleanup() {
  #cycle the log before each run
  cat /dev/null > $LOG_FILE
  rm $CLANAVI_DIR/bundle_instance.* 
}

# Write output to log file
log() {
  if [ "$DEBUG" -eq 1 ]; then
    echo $1 >> $LOG_FILE
  fi;
}

# Log error status
log_error() {
  echo -e $1;
  log $1
  updatestatus "failed" 
  exit 1;
}

# Set Environment
check_env() {
  log "Check for Amazon EC2 toolkit"

  # Tools already exist
  if [ -z `which ec2-upload-bundle` ] ; then
    log "Installing Amazon EC2 toolkit."
    install_ec2
  fi
}

# Install EC2 libraries
install_ec2() {
  log "Installing system packages"  
  for PACKAGE in wget tar bzip2 unzip zip symlinks unzip ruby openssl libopenssl-ruby; do
    if ! which "$PACKAGE" >/dev/null; then
      sudo $PACKAGE_MANAGEMENT install -y $PACKAGE
    fi
  done;

  log "Setup AMI Tools"  

  # Unpack and install AMI Tools
  if [ -z "`which ec2-upload-bundle`" ]; then
    # Download the ami tool into temporary directory
    curl -o /tmp/ec2-ami-tools.zip $CURL_OPTS $EC2_AMI_TOOL_URL
    
    # Clean out the old EC2 AMI Tools directory if it exists
    sudo rm -rf $EC2_HOME;
    
    cd /usr/local && sudo unzip /tmp/ec2-ami-tools.zip
    sudo ln -svf `find . -type d -name ec2-ami-tools*` ec2-ami-tools
    
    # Cleanup temporary directory    
    rm -rvf /tmp/ec2*
    sudo ln -sf $EC2_HOME/bin/* /usr/bin/
    sudo rm -f /usr/bin/ec2-*.cmd
  fi
  
}

# Main Bundle Logic
bundle() {
  updatestatus "initiated" 
  log "Started the Bundling Instance Process"

  log "Starting ec2-bundle-vol"
  sudo -E /usr/bin/ec2-bundle-vol \
    -r $arch \
    -d /mnt \
    -p $IMAGE_NAME \
    -u $AWS_USER_ID \
    -k $EC2_PRIVATE_KEY_FILE \
    -c $EC2_CERT_FILE \
    -s $SIZE \
    -e /mnt,/root/.ssh,$EC2_HOME_DIR >> $LOG_FILE
  RESULT=$?
  if [ $RESULT -eq 0 ];then
     log "ec2-bundle-vol : Success"
  else
     log "ec2-bundle-vol : Failed $RESULT "
     updatestatus "failed" 
     exit 125
  fi
  
  log "Starting ec2-upload-bundle"
  sudo -E /usr/bin/ec2-upload-bundle \
    -l $REGION \
    -b $BUCKET \
    -m /mnt/$IMAGE_NAME.manifest.xml \
    -a "$AWS_ACCESS_KEY_ID" \
    -s "$AWS_SECRET_ACCESS_KEY" >> $LOG_FILE
  RESULT=$?
  if [ $RESULT -eq 0 ];then
     log "ec2-upload-bundle : Success"
  else
     log "ec2-upload-bundle : Failed"
     updatestatus "failed" 
     exit 126
  fi
  
  log "Completed the Bundling Instance Process Successfully"
  rm -fr $EC2_HOME_DIR
  updatestatus "success"
}

  
# Check Environment
check_env

# Run the script
bundle








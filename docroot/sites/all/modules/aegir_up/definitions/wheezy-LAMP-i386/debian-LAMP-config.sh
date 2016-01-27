# Install git
apt-get install git-core -y

# Download Aegir-up locally
git clone http://git.drupal.org/project/aegir-up.git

# Install LAMP stack
puppet apply ./aegir-up/definitions/debian-LAMP/debian-LAMP.pp --modulepath="./aegir-up/modules"

# Clean up
rm -rf ./aegir-up

exit

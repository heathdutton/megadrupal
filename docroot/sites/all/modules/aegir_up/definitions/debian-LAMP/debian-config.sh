# Remove boot menu delay
echo 'GRUB_HIDDEN_TIMEOUT=true' >> /etc/default/grub
sed 's/GRUB_TIMEOUT=5/GRUB_TIMEOUT=0/g' -i /etc/default/grub
update-grub

# Install git
apt-get install git-core -y

# Download Aegir-up locally
git clone --branch 7.x-0.x http://git.drupal.org/project/aegir-up.git

# Install basic packages
puppet apply ./aegir-up/lib/definitions/debian/debian.pp --modulepath="./aegir-up/lib/modules"

exit

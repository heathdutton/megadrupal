#/bin/sh

curl https://raw.githubusercontent.com/GetValkyrie/ansible-bootstrap/master/install-ansible.sh 2>/dev/null | /bin/sh
ansible-galaxy install --ignore-errors http://github.com/getvalkyrie/ansible-role-aegir,,getvalkyrie.aegir
ansible-galaxy install --ignore-errors http://github.com/getvalkyrie/ansible-role-mysql,,getvalkyrie.mysql
target=/etc/ansible/roles/getvalkyrie.skynet
if [ ! -e $target ]; then
  mkdir -p /etc/ansible/roles/
  ln -s /vagrant/ $target
fi
 

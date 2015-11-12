#/bin/sh

curl https://raw.githubusercontent.com/GetValkyrie/ansible-bootstrap/master/install-ansible.sh 2>/dev/null | /bin/sh
ansible-galaxy install --ignore-errors http://github.com/getvalkyrie/ansible-role-aegir,,getvalkyrie.aegir
ansible-galaxy install --ignore-errors http://github.com/getvalkyrie/ansible-role-skynet,,getvalkyrie.skynet
target=/etc/ansible/roles/getvalkyrie.valkyrie
if [ ! -e $target ]; then
  ln -s /vagrant/ $target
fi
 

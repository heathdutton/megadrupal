# Ansible Role: Valkyrie

[![Build Status](https://travis-ci.org/GetValkyrie/ansible-role-valkyrie.svg?branch=master)](https://travis-ci.org/GetValkyrie/ansible-role-valkyrie)

Installs Aegir and various extensions to provide an end-to-end
Git/Features-based Drupal development workflow.

## Requirements

None.

## Role Variables

Available variables are listed below, along with default values (see `defaults/main.yml`):


## Dependencies

  - getvalkyrie.aegir (Installs Aegir).

## Example Playbook

    - hosts: servers
      roles:
        - { role: getvalkyrie.valkyrie }

After the playbook runs, the Aegir front-end site will be available, as will
the Drush extensions (Provision, et. al.) that do the heavy lifting.

## License

MIT / BSD

## Author Information

This role was created in 2015 by [Christopher Gervais](http://ergonlogic.com/), lead maintainer of the [Aegir Hosting System](http://www.aegirproject.org).

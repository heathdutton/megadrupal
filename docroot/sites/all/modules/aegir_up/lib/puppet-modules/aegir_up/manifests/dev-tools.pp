# Class to add some developer tools

class aegir-up::dev-tools {

  # For Simpletest
  package {'php5-curl':
    ensure => installed,
    }

  package {'vim':
    ensure => installed,
    }
  
  #Pathogen (ref.: https://github.com/tpope/vim-pathogen)
  file { ['/var/aegir/.vim', '/var/aegir/.vim/autoload', '/var/aegir/.vim/bundle' ]:
    ensure => directory,
    }
  exec { '/usr/bin/curl -so /var/aegir/.vim/autoload/pathogen.vim https://raw.github.com/tpope/vim-pathogen/HEAD/autoload/pathogen.vim':
    require => File['/var/aegir/.vim/autoload'],
    }

  # Drupal-specific vim plugins (ref.: http://drupal.org/node/1389006)
  # Installation: http://drupal.org/node/1389448
  exec { '/usr/bin/git clone --branch 7.x-1.x http://git.drupal.org/project/vimrc.git /var/aegir/.vim/bundle/drupal_vimrc':
    require => File['/var/aegir/.vim/bundle'],
  }

  # Solarized colour-scheme for Vim (ref.: http://ethanschoonover.com/solarized)
  exec { '/usr/bin/git clone git://github.com/altercation/vim-colors-solarized.git /var/aegir/.vim/bundle/vim-colors-solarized':
    require => File['/var/aegir/.vim/bundle'],
  }

  # Solarized colour-scheme for terminal (ref.: http://ethanschoonover.com/solarized)
  exec { '/usr/bin/curl -so /home/vagrant/.Xdefaults https://raw.github.com/altercation/solarized/master/xresources-colors-solarized/Xresources': }

  file {'/var/aegir/.vimrc':
    ensure => present,
    source => 'puppet:///modules/aegir-up/vimrc'
  }


}

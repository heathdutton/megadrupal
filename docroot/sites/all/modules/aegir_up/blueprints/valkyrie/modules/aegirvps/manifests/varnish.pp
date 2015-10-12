# N.B. $varnish_vcls, $varnish_backends & $aegirvps_dir are set by custom facts
# shipped with this module. See: lib/facter/varnish.rb & lib/facter/aegirvps.rb

# Add a backend define to a node manifest to install and configure Varnish.
# Each backend gets its own .vcl (vagrant.d/backend_*.vcl)
# See: aegirvps::varnish::backend in varnish/backend.pp

# Varnish installation and configuration. Should not be called directly.
# Use aegirvps::varnish::backend instead
class aegirvps::varnish {

  # Install varnish package
  package {'varnish':
    ensure => installed,
  }

  # Create our varnish.d directory in the client directory
  file {'varnish.d':
    ensure => directory,
    path   => "${aegirvps_dir}/varnish.d",
  }

  # Copy our default config file to the client directory
  file {'varnish.config':
    ensure  => file,
    # We want changes made in the client's repo to persist, so we won't replace
    replace => false,
    path    => "${aegirvps_dir}/varnish.d/varnish.config",
    source  => 'puppet:///modules/aegirvps/varnish/varnish.config',
    require => File['varnish.d'],
  }

  # Drop our config file in place
  file {'/etc/default/varnish':
    ensure  => file,
    owner   => 'root',
    group   => 'root',
    source  => "${aegirvps_dir}/varnish.d/varnish.config",
    require => File['varnish.config'],
  }

  # Generate some config files that'll go in <aegirvps-clients path>/varnish.d/
  # Use 'replace => false' to avoid having changes overwritten.
  # Deleting any of the generated config files should have it regenerated
  # TODO: These files ought to have some embedded documentation

    # Generic default Drupal Varnish config
  file {'drupal.vcl':
    ensure  => file,
    replace => false,
    path    => "${aegirvps_dir}/varnish.d/drupal.vcl",
    source  => "puppet:///modules/aegirvps/varnish/drupal.vcl",
    require => File['varnish.d'],
  }

    # Default 503 error page
  file {'error.vcl':
    ensure  => file,
    replace => false,
    path    => "${aegirvps_dir}/varnish.d/error.vcl",
    source  => "puppet:///modules/aegirvps/varnish/error.vcl",
    require => File['varnish.d'],
  }

    # The director handles load-balancing across backends
    # Puppet strangeness requires this: http://projects.puppetlabs.com/issues/15718
  define varnish_director ($backends = ['default'], $type = 'round-robin') {
    file { $name :
      ensure  => file,
      path    => "${aegirvps_dir}/varnish.d/director.vcl",
      content => template('aegirvps/varnish_director.erb'),
      require => File['varnish.d'],
    }
  }
  # Generate an array from the list we get from our custom fact
  $backends = split($varnish_backends, ',')
  # Generate our director.vcl based on this list
  varnish_director { 'aegirvps': backends => $backends }
  # TODO: Add runstage to ensure this is generated after backends? Thus avoiding the
  # necessity for multiple puppet runs.

  # TODO: Commit initial generated configs to git
  # Use 'refreshonly => true' since we won't want to commit any user changes
  # We'll want to subscribe to the appropriate Files[] though, so that we re-
  # commit any re-generated configs

  # Generate /etc/varnish/default.vcl as a list of include lines. We can create
  # a custom fact that reads our varnish.d/, and outputs a comma-delimited
  # string of filenames. We can then iterate over this using the line() function

  file {'/etc/varnish/default.vcl':
    ensure  => file,
    content => "### Managed by Puppet.\n\n",
    require => Package['varnish'],
  }

  # Puppet strangeness requires this: http://projects.puppetlabs.com/issues/15718
  define aegirvps::varnish_vcl {
    # So that grep, etc. work in line{}
    Exec { path => "/bin" }

    if 'backend_' in $name {
      $before = Line['director.vcl']
    }

    line { $name:
     	file    => '/etc/varnish/default.vcl',
 	    line    => "include \"${aegirvps_dir}/varnish.d/${name}\";",
      require => File['/etc/varnish/default.vcl'],
      before  => $before,
    }
  }
  # Generate an array from the list we get from our custom fact
  $vcls = split($varnish_vcls, ',')
  # Add them as includes to our default.vcl
  aegirvps::varnish_vcl { $vcls : }
  # TODO: Add runstage to ensure this is generated last? Thus avoiding the
  # necessity for multiple puppet runs.

  if $local_dev {  # Note: this is a global variable set in site.pp
    # TODO: For development using drush-vagrant, we can set entries in
    # /etc/hosts on the varnish VM. They will point to varnish backends (e.g.
    # web node VMs) in the project so that varnish checks these local VMs
    # instead of production hosts. The point is not to have to modify the .vcls
    # themselves.
  }

}


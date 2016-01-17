# N.B. $varnish_vcls, $varnish_backends & $aegirvps_dir are set by custom facts
# shipped with this module. See: lib/facter/varnish.rb & lib/facter/aegirvps.rb

# Add a backend define to a node manifest to install and configure Varnish.
# Each backend gets its own .vcl (vagrant.d/backend_*.vcl)
define aegirvps::varnish::backend ($host, $port, $probe = undef) {
  include aegirvps::varnish
  file { $name :
    ensure  => file,
    path    => "${aegirvps_dir}/varnish.d/backend_$name.vcl",
    content => template('aegirvps/varnish_backend.erb'),
    require => File['varnish.d'],
  }
}


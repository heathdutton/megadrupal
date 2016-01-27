# Usage:
# aegirvps::kplatforms {['Drupal6','Drupal7','CiviCRM4','CiviCRM4-D6']:}

define aegirvps::kplatforms (
  $release     = $kplatforms_release,
  $commit      = $kplatforms_commit,
  $local_build = false) {

  # Don't build platforms in local dev environments, unless specifically told
  # to, but provide help for those who may want to.
  if ($local_dev and !$local_build) {
    warning( "Skipping build of kPlatforms since we're running in a local dev environment.\n Add the 'local_build => 'true' parameter to aegirvps::kplatforms in order to build these platforms locally." )
  }
  # But otherwise do!
  else {
    aegirvps::kplatforms::platform {$name :
      release => $release,
      commit => $commit,
    }
  }

}


define aegirvps::kplatforms::platform (
  $release = $kplatforms_release,
  $commit  = $kplatforms_commit) {

  $makefile = "http://drupalcode.org/project/kplatforms.git/blob_plain/${commit}:/stubs/${name}.make"

  aegir::platform {"k${name}_build-${release}":
    makefile      => $makefile,
    # Currently, if the build times out, drush make continues to build the
    # platform. This then blocks further attempts, as it'll fail on the next
    # run, since the target directory already exists.
    # TODO: Fix in aegir::platform, perhaps with an 'unless' that checks for
    # the existence of the platform name in Aegir (via 'drush eval')
    build_timeout => '0',
  }
}

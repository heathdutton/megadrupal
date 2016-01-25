class Vm                                 # default virtual machine settings
  def self.descendants
    ObjectSpace.each_object(::Class).select {|klass| klass < self }
  end

  Count     = 1                          # number of VMs to create
  Basebox   = "debian-LAMP-i386-2012-03-29"  # default basebox
  Box_url   = "http://ergonlogic.com/files/boxes/debian-LAMP-i386-current.box"
  Memory    = 512                        # default VM memory
  Domain    = "local"                    # default domain
  Manifests = "manifests"                # puppet manifests folder name
  Modules   = {}                         # hash of puppet module folder names
  Site      = "site"                     # name of manifest to apply
  Gui       = false                      # start VM with GUI?
  Verbose   = false                      # make output verbose?
  Debug     = false                      # output debug info?
  Options   = ""                         # options to pass to Puppet
  Facts     = {}                         # hash of Factor facts
  SSH_tries = 5                          # How quickly to fail should Vagrant hang
  SSH_forward_agent = false              # Whether to forward SSH agent
end

class Global
  Network   = "192.168"                  # Private network address: ###.###.0.0
  Host_IP   = 10                         # Starting host address: 192.168.0.###
  SSH_range = (32200..32250)
end

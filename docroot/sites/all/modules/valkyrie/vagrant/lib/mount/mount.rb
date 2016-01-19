# These faux plugins make NFS mount in an easy read-write capacity.

module VagrantPlugins
  module Valkyrie
    module Action
      class Mount

        def initialize(app, env)
          @app = app
          @machine = env[:machine]
          @ui = env[:ui]
          @logger = Log4r::Logger.new("ValkyrieMount::action::ValkyrieMount")
          @valkyrie_vms = ENV.fetch('VALKYRIE_VMS', 'valkyrie').split(',')
          @plugin_dir = File.expand_path(File.dirname(__FILE__))
          @project_path = ENV.fetch('VALKYRIE_PROJECT_PATH', '.')
          @semaphore_path = @project_path+'/.valkyrie/cache/first_run_complete'
          @current_ssh_username = @machine.config.ssh.username
        end

        def call(env)
          if valkyrie_vm?
            if env[:machine_action] == :up
              @ui.info "Checking NFS user/group mapping."
              if !File.exist?(@semaphore_path)
                @ui.info "Fixing NFS user/group mapping."
                @ui.detail "Setting up SSH access for the 'ubuntu' user."
                grant_ssh_access
                @ui.detail "Refreshing SSH connection, to login as 'ubuntu'."
                refresh_ssh_connection('ubuntu')
                @ui.detail "Installing Ansible from sources."
                install_ansible
                @ui.detail "Running Ansible playbook to re-map users and groups."
                run_playbook
                @ui.detail "Refreshing SSH connection, to login normally."
                refresh_ssh_connection(@current_ssh_username)
                @ui.detail "Writing semaphore file."
                write_semaphore
              end
            end
          end
          # Proceed with the rest of the middleware stack
          @app.call(env)
        end

        def grant_ssh_access
          @machine.communicate.sudo("cp /home/vagrant/.ssh/authorized_keys /home/ubuntu/.ssh/authorized_keys")
          @machine.communicate.sudo("chown ubuntu:ubuntu /home/ubuntu/.ssh/authorized_keys")
          @machine.communicate.sudo("chmod 600 /home/ubuntu/.ssh/authorized_keys")
        end

        def install_ansible
          ansible_bootstrap = "https://raw.githubusercontent.com/GetValkyrie/ansible-bootstrap/master/install-ansible.sh"
          install_ansible = "curl -s #{ansible_bootstrap} | /bin/sh"
          @machine.communicate.sudo(install_ansible) do |type, data|
            if !data.chomp.empty?
              @machine.ui.info(data.chomp)
            end
          end
        end

        def run_playbook
          @machine.communicate.upload(@plugin_dir+'/mount.yml', "/tmp/mount.yml")
          @machine.communicate.upload(@plugin_dir+'/inventory', "/tmp/inventory")
          ansible_playbook = "PYTHONUNBUFFERED=1 "
          ansible_playbook << "ANSIBLE_FORCE_COLOR=true "
          ansible_playbook << "ansible-playbook "
          ansible_playbook << "/tmp/mount.yml "
          ansible_playbook << "-i /tmp/inventory "
          ansible_playbook << "--connection=local "
          ansible_playbook << "--sudo "
          ansible_playbook << "--extra-vars \""
          ansible_playbook << "host_os="+RUBY_PLATFORM+' '
          ansible_playbook << "host_gid="+Process.gid.to_s+' '
          ansible_playbook << "host_uid="+Process.uid.to_s+' '
          ansible_playbook << "\""
          @machine.communicate.sudo(ansible_playbook) do |type, data|
            if !data.chomp.empty?
              @machine.ui.info(data.chomp)
            end
          end
        end

        def refresh_ssh_connection(username)
          @machine.communicate.instance_variable_get(:@connection).close
          @machine.config.ssh.username = username
        end

        def write_semaphore
          cache_path = @project_path+'/.valkyrie/cache'
          system("mkdir -p #{cache_path}")
          system("date > #{@semaphore_path}")
        end

        def valkyrie_vm?
          @valkyrie_vms.include?(@machine.name.to_s)
        end

      end

      class RemoveSemaphore < Mount

        def call(env)
          if valkyrie_vm?
            if env[:machine_action] == :destroy
              if File.exist?(@semaphore_path)
                @ui.detail "Removing semaphore"
                delete_semaphore
              end
            end
          end
          # Proceed with the rest of the middleware stack
          @app.call(env)
        end

        def delete_semaphore
          File.delete(@semaphore_path)
        end

      end
    end
  end
end

module VagrantPlugins
  module Valkyrie
    class MountPlugin < Vagrant.plugin('2')
      name 'ValkyrieMount'
      description <<-DESC
        Improve NFS workflows for local dev.
      DESC

      action_hook("Valkyrie", :machine_action_up) do |hook|
        hook.append(Action::Mount)
      end

      action_hook("Valkyrie", :machine_action_destroy) do |hook|
        hook.append(Action::RemoveSemaphore)
      end
    end
  end
end

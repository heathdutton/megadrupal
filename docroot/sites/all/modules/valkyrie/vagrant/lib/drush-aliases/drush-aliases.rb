# These faux plugins Add and remove Drush alias search paths.

module VagrantPlugins
  module Valkyrie
    module Action
      class AddAliasSearchpath

        def initialize(app, env)
          @app = app
          @machine = env[:machine]
          @ui = env[:ui]
          @logger = Log4r::Logger.new("ValkyrieMount::action::ValkyrieMount")
          @valkyrie_vms = ENV.fetch('VALKYRIE_VMS', 'valkyrie').split(',')
          @project_path = ENV.fetch('VALKYRIE_PROJECT_PATH', File.dirname(File.expand_path(__FILE__)))
          @project_alias_path = ENV.fetch('VALKYRIE_PROJECT_ALIAS_PATH', @project_path+'/.valkyrie/cache/aliases')
          @project_alias_include_line = "$options['alias-path'][] = '#{@project_alias_path}';"
          @drushrc_path = ENV.fetch('VALKYRIE_DRUSHRC_PATH', ENV['HOME']+'/.drushrc.php')
          @drushrc_include_path = ENV.fetch('VALKYRIE_DRUSHRC_INCLUDE_PATH', ENV['HOME']+'/.valkyrie.drushrc.php')
          @drushrc_include_line = "include_once('#{@drushrc_include_path}');"
        end

        def call(env)
          if valkyrie_vm?
            if [:up, :resume, :reload].include?(env[:machine_action])
              @ui.info 'Checking Drush alias integration.'
              ensure_drushrc_exists
              ensure_drushrc_include_exists
              ensure_drushrc_included
              ensure_project_alias_path_included
            end
          end
          # Proceed with the rest of the middleware stack
          @app.call(env)
        end

        def ensure_drushrc_exists
          if File.exist?(@drushrc_path)
            @ui.detail "Found drushrc file (#{@drushrc_path})."
          else
            @ui.detail "Creating new drushrc file (#{@drushrc_path})."
            File.open(@drushrc_path,'w') {|file| file.puts "<?php\n"}
          end
        end

        def ensure_drushrc_include_exists
          if File.exist?(@drushrc_include_path)
            @ui.detail "Found Valkyrie drushrc include file (#{@drushrc_include_path})."
          else
            @ui.detail "Creating new Valkyrie drushrc include file (#{@drushrc_include_path})."
            File.open(@drushrc_include_path,'w') {|file| file.puts "<?php\n"}
          end
        end

        def drushrc_included
          if File.exists?(@drushrc_path)
            File.open(@drushrc_path,'r').each_line do |line|
              if line.chomp == @drushrc_include_line
                return true
              end
            end
          end
          return false
        end

        def ensure_drushrc_included
          if drushrc_included
            @ui.detail "Found Valkyrie drushrc include statement in drushrc file (#{@drushrc_path})."
          else
            @ui.detail "Adding Valkyrie drushrc include statement to drushrc file (#{@drushrc_path})."
            File.open(@drushrc_path, 'a') do |file|
              file.puts @drushrc_include_line
            end
          end
        end

        def project_alias_path_included
          if File.exists?(@drushrc_include_path)
            File.open(@drushrc_include_path,'r').each_line do |line|
              if line.chomp == @project_alias_include_line
                return true
              end
            end
          end
          return false
        end

        def ensure_project_alias_path_included
          if project_alias_path_included
            @ui.detail "Found project alias path in Valkyrie drushrc include file (#{@drushrc_include_path})."
          else
            @ui.detail "Adding project alias path to Valkyrie drushrc include file (#{@drushrc_path})."
            File.open(@drushrc_include_path, 'a') do |file|
              file.puts @project_alias_include_line
            end
          end
        end

        def valkyrie_vm?
          @valkyrie_vms.include?(@machine.name.to_s)
        end

      end

      class RemoveAliasSearchpath < AddAliasSearchpath

        def call(env)
          if valkyrie_vm?
            if [:destroy, :suspend, :halt].include?(env[:machine_action])
              ensure_project_alias_path_removed
              if env[:machine_action] == :destroy
                delete_project_alias_path
              end
            end
          end
        end

        def ensure_project_alias_path_removed
          if project_alias_path_included
            @ui.detail "Removing project alias path in Valkyrie drushrc include file (#{@drushrc_include_path})."
            require 'fileutils'
            require 'tempfile'
            tmp = Tempfile.new("extract")
            open(@drushrc_include_path, 'r').each { |l| tmp << l unless l.chomp == @project_alias_include_line }
            tmp.close
            FileUtils.mv(tmp.path, @drushrc_include_path)
          end
        end

        def delete_project_alias_path
          if File.exists?(@project_alias_path)
            @ui.detail "Deleting project alias path (#{@project_alias_path})."
            require 'fileutils'
            FileUtils.rm_r(@project_alias_path)
          end
        end

      end
    end
  end
end

module VagrantPlugins
  module Valkyrie
    class Plugin < Vagrant.plugin('2')
      name 'ValkyrieDrushAliases'
      description <<-DESC
        Add and remove locations for Drush to search for aliases.
      DESC

      ['up', 'resume', 'reload'].each do |action|
        action_hook("Valkyrie", "machine_action_#{action}") do |hook|
          hook.append(Action::AddAliasSearchpath)
        end
      end

      ['destroy', 'suspend', 'halt'].each do |action|
        action_hook("Valkyrie", "machine_action_#{action}") do |hook|
          hook.append(Action::RemoveAliasSearchpath)
        end
      end
    end
  end
end

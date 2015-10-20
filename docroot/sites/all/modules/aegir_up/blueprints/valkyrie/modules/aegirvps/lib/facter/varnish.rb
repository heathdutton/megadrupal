# Get the location of our AegirVPS client's repo that' s set by another custom
# fact
aegirvps_dir = Facter.value('aegirvps_dir')

# Only set these facts once there are VCLs in varnish.d
if File.directory?("#{aegirvps_dir}/varnish.d")

    # List *all* VCLs
    Facter.add("varnish_vcls") do
        setcode do
            Dir.chdir("#{aegirvps_dir}/varnish.d")
            Dir.glob("*.vcl").join(",")
        end
    end

    # List just the backend VCLs
    Facter.add("varnish_backends") do
        setcode do
            Dir.chdir("#{aegirvps_dir}/varnish.d")
            vcls = Dir.glob("backend_*.vcl")
            vcls.each do |vcl|
              vcl.sub!('backend_', '')
              vcl.sub!('.vcl', '')
            end
            vcls.join(",")
        end
    end

end

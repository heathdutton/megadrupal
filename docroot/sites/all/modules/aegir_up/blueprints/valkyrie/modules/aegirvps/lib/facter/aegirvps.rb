aegirvps_dir = "/etc/aegirvps"

Facter.add("aegirvps_dir") do
    setcode do
        if File.directory?(aegirvps_dir)
            aegirvps_dir
        end
    end
end


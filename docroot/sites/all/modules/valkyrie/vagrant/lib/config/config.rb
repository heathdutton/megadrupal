require 'yaml'

def load_config(configs, ref_path = '.')
  conf = Hash.new
  configs.each do | config |
    config_path = ref_path+'/'+config
    if File.exist?(config_path)
      conf.deep_merge!(YAML.load_file(config_path))
    end
  end

  return conf
end

# Shamelessly stolen from Rails4: http://apidock.com/rails/v4.1.8/Hash/deep_merge%21
class Hash
  def deep_merge!(other_hash, &block)
    other_hash.each_pair do |current_key, other_value|
      this_value = self[current_key]

      self[current_key] = if this_value.is_a?(Hash) && other_value.is_a?(Hash)
        this_value.deep_merge!(other_value, &block)
      else
        if block_given? && key?(current_key)
          block.call(current_key, this_value, other_value)
        else
          other_value
        end
      end
    end

    self
  end
end

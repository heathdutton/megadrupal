css_dir = "css"
sass_dir = "sass"
fonts_dir = "fonts"
images_dir = "images"
javascripts_dir = "js"

environment = :production

if environment == :production
	output_style = :compact
else
	output_style = :expanded
	sass_options = {:debug_info => true}
end

# To disable debugging comments that display the original location of your selectors. Uncomment:
# line_comments = false

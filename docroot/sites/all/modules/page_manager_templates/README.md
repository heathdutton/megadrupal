> Prerequisites: An understanding of CTools' Page Manager and the Drupal Theming system is assumed.

#Creating a Template page

You can add a new page to your site which will output the contents of a new template file you can write.

On admin/structure/pages, add or edit a page manager page. Choose a variant type of "Template".

### Provide Theme Function?
The default setting is to let Page Manager Templates provide the hook_theme function. If you will be creating a new template file (typical use), leave this checked. If you are using an existing theme function or template file, or for some other reason want to handle your own hook_theme implementation, you can uncheck the box. The following instructions assume you have left this setting checked.

### Template Name?
Type in the name of your new template file. For example, enter home-page in this field to use a template file named home-page.tpl.php.

###Will a module or theme contain your template file?
You can put your new template file in either a module or a theme. Typically you should do this in a custom module or custom theme. Choose whether you prefer to use a module or theme, then choose which module or theme you will use.

###Subdirectory?
If you are not placing your new template in the root of the module or theme directory you chose, enter the subdirectory path here. For example, templates or templates/home.

###Override entire page?
Do you want to control the *entire* page (not just the content region) and include the entire html file in your new template? Down with all Drupal markup! You're in charge, cowboy!

###Create your template file
If you haven't yet, create the new template in the place you indicated. If you chose template name of home-page, type of theme, them of custom_theme, and subdirectory of templates, you need to create a new file at custom_theme/templates/home-page.tpl.php.

Add some test text to the template file. You should see it at the path you chose for your page.

###What variables are available?
To see which variables are available, you can add this to your template file:
```
<?php dpm(get_defined_vars()); ?>
```
> You can use contexts and arguments as with any page manager page. The variables will contain the contexts you use.

###Are these variables sanitized?
Do not assume anything is sanitized for security. If you are outputting user-generated content you are responsible to sanitize it in your template file, or in a preprocess function.

###How do I use a preprocess function?
Your Page Manager's Variant Summary section will show you the name of the preprocess function you can use in your module or in your theme's template.php file. This is where you can add javascript and css files. It is recommended to finess your variables here into a more ideal format before you template them.

#Creating a Template Panels pane
You can add a new pane to a Panel which will output the contents of a new template file you can write.

On the Content section of the Variant's settings, add new content to a region. Choose the Template content type from the Template category.

The settings are the same as above. See "Creating a Template Page"

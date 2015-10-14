/*
 @file
 */

(function($)
{
    Drupal.behaviors.lr_social_profile_data = {
        attach: function(context, settings)
        {
            // Create a container div for the modal if one isn't there already
            if ($("#lr_social_profile_data-container").length == 0)
            {
                // Add a container to the end of the body tag to hold the dialog
                $('body').append('<div id="lr_social_profile_data-container" style="display:none;"></div>');
                try
                {
                    $("#lr_social_profile_data-container", context).dialog({
                        autoOpen: false,
                        modal: true,
                        close: function(event, ui)
                        {
                            // Clear the dialog on close. Not necessary for your average use
                            // case, butis useful if you had a video that was playing in the
                            // dialog so that it clears when it closes
                            $('#lr_social_profile_data-container').html('');
                        }
                    });
                    var defaultOptions = Drupal.lr_social_profile_data.explodeOptions(settings.lr_social_profile_data.defaults);
                    $('#lr_social_profile_data-container').dialog('option', defaultOptions);
                }
                catch (err)
                {
                    // Catch any errors and report
                    Drupal.lr_social_profile_data.log('[error] lr_social_profile_data Dialog: ' + err);
                }
            }
            // Add support for custom classes if necessary
            var classes = '';
            if (settings.lr_social_profile_data.classes)
            {
                classes = ', .' + settings.lr_social_profile_data.classes;
            }
            $('a.lr_social_profile_data' + classes, context).each(function(event)
            {
                if (!event.metaKey && !$(this).hasClass('lr_social_profile_dataProcessed'))
                {
                    // Add a class to show that this link has been processed already
                    $(this).addClass('lr_social_profile_dataProcessed');
                    $(this).click(function(event)
                    {
                        // prevent the navigation
                        event.preventDefault();
                        // Set up some variables
                        var url = $(this).attr('href');
                        var title = $(this).attr('title') ? $(this).attr('title') : settings.lr_social_profile_data.title;
                        if (!title)
                        {
                            title = $(this).text();
                        }
                        // Use defaults if not provided
                        var selector = $(this).attr('name') ? $(this).attr('name') : settings.lr_social_profile_data.selector;
                        var options = $(this).attr('rel') ? Drupal.lr_social_profile_data.explodeOptions($(this).attr('rel')) : Drupal.lr_social_profile_data.explodeOptions(settings.lr_social_profile_data.defaults);
                        if (url && title && selector)
                        {
                            // Set the custom options of the dialog
                            $('#lr_social_profile_data-container').dialog('option', options);
                            // Set the title of the dialog
                            $('#lr_social_profile_data-container').dialog('option', 'title', title);
                            // Add a little loader into the dialog while data is loaded
                            $('#lr_social_profile_data-container').html('<div class="lr_social_profile_data-ajax-loader"></div>');
                            // Change the height if it's set to auto
                            if (options.height && options.height == 'auto')
                            {
                                $('#lr_social_profile_data-container').dialog('option', 'height', 200);
                            }
                            // Use jQuery .get() to request the target page
                            $.get(url, function(data)
                            {
                                // Re-apply the height if it's auto to accomodate the new content
                                if (options.height && options.height == 'auto')
                                {
                                    $('#lr_social_profile_data-container').dialog('option', 'height', options.height);
                                }
                                // Some trickery to make sure any inline javascript gets run.
                                // Inline javascript gets removed/moved around when passed into
                                // $() so you have to create a fake div and add the raw data into
                                // it then find what you need and clone it. Fun.
                                $('#lr_social_profile_data-container').html($('<div></div>').html(data).find('#' + selector).clone());
                                // Attach any behaviors to the loaded content
                                Drupal.attachBehaviors($('#lr_social_profile_data-container'));
                            });
                            // Open the dialog
                            $('#lr_social_profile_data-container').dialog('open');
                            // Return false for good measure
                            return false;
                        }
                    });
                }
            });
        }
    }
    Drupal.lr_social_profile_data = {};

    // Convert the options to an object
    Drupal.lr_social_profile_data.explodeOptions = function(opts)
    {
        var options = opts.split(';');
        var explodedOptions = {};
        for (var i in options)
        {
            if (options[i])
            {
                // Parse and Clean the option
                var option = Drupal.lr_social_profile_data.cleanOption(options[i].split(':'));
                explodedOptions[option[0]] = option[1];
            }
        }
        return explodedOptions;
    }

    // Function to clean up the option.
    Drupal.lr_social_profile_data.cleanOption = function(option)
    {
        // If it's a position option, we may need to parse an array
        if (option[0] == 'position' && option[1].match(/\[.*,.*\]/))
        {
            option[1] = option[1].match(/\[(.*)\]/)[1].split(',');
            // Check if positions need be converted to int
            if (!isNaN(parseInt(option[1][0])))
            {
                option[1][0] = parseInt(option[1][0]);
            }
            if (!isNaN(parseInt(option[1][1])))
            {
                option[1][1] = parseInt(option[1][1]);
            }
        }
        // Convert text boolean representation to boolean
        if (option[1] === 'true')
        {
            option[1] = true;
        }
        else
        {
            if (option[1] === 'false')
            {
                option[1] = false;
            }
        }
        return option;
    }

    Drupal.lr_social_profile_data.log = function(msg)
    {
        if (window.console)
        {
            window.console.log(msg);
        }

    }

})(jQuery);

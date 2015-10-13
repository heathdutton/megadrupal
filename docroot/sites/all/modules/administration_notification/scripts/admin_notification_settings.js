(function($)
{
	Drupal.behaviors.adminNotification = 
	{
		attach: function(context, settings)
		{
			var email = Drupal.settings.adminNotification.email;
			$("#edit-admin-notification-email").focus(function()
			{
				if($(this).val() == email)
				{
					$(this).val("");
				}
			});
			$("#edit-admin-notification-email").blur(function()
			{
				if($(this).val() == "")
				{
					$(this).val(email);
				}
			});
		}
	};
})(jQuery);
jQuery(document).ready(function($)
	{
		$.current_selected = $("#first_selected").html();
		$("[id^='feature-href-']").click(function()
			{
				$use_id = $(this).attr("id").replace("feature-href-", "");
				
				$old_header = "#feature-node-header-"+$.current_selected;
				$old_media = "#feature-node-media-"+$.current_selected;
				$header_id = "#feature-node-header-"+$use_id;
				$media_id = "#feature-node-media-"+$use_id;
				
				$("#feature-media-container").slideUp("slow");
				$($media_id+" > object").addClass("no_display");
				
				/* Clear old header */
				$($old_header).addClass("no_display");
				$($header_id).removeClass("no_display");
				
				/* Hide old Media*/
				$($old_media).slideUp("slow");
				$($old_media+" object").addClass("no_display");
				
				setTimeout(function()
					{
						$("#feature-media-container").slideDown("fast");				
						setTimeout(function()
							{	
								$($media_id).slideDown("slow");		
								$($media_id+" > object").removeClass("no_display");
							}
						,1000);
					}
				,150);
				$.current_selected = $use_id;
				return false;
			});
	});
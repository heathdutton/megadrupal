
(function ($) {

/**
 * This script reads through feed settings and fetches feed data using the Google FeedAPI
 */
Drupal.behaviors.googleFeedAPI = {
  attach: function (context, settings) {
	  if (typeof google == 'undefined'){
		  return;
	  }
	  
	  //Loop through the feeds that are on this page
	  $.each(settings.googleFeedAPI, function(div_id, feed_setting) { 
		  
		  //Run the feed processing only once per feed
		  $('div#'+div_id, context).once('googleFeedAPI', function () {
			    //Load Feed
			    var container = $(this);
			    var feed = new google.feeds.Feed(feed_setting.url);
			    feed.setNumEntries(feed_setting.num_feeds);
		        feed.load(function(result) {
		          if (!result.error) {
		            for (var i = 0; i < result.feed.entries.length; i++) {
		           	  var entry = result.feed.entries[i];
		           	  
		           	  var date = "";
		           	  if(typeof entry.publishedDate != 'undefined' && entry.publishedDate != ''){
		           		if(feed_setting.time_display == 'relative'){  
		           		  //@todo find a good way to do FuzzyTime in js
		           		  date = googleFeedAPIFuzzyDate(entry.publishedDate);
		           	    }
		           		
		           		if(feed_setting.time_display == 'formal'){  
			              date = googleFeedAPIFormalDate(entry.publishedDate);
			           	}
		           		
		           		if(typeof date == 'undefined'){
		           		  date = "";
		           		}else{
		           		  date = "<span class='date'>" + date + "</span>";
		           		}
		           	  }
		           	  
		           	  var content = entry.content;
		           	
		           	  if(feed_setting.content_summary && typeof entry.contentSnippet != 'undefined'){
		           		content = entry.contentSnippet;
		           	  }
		           	  
		           	  var feed_markup = "<div class='feed_item'>";
		           	  
		           	  //Put time before title if there is no content
		           	  if(!feed_setting.show_content){
		           		feed_markup = feed_markup + date;
		           	  }
	            	
		           	  //Add Title
	            	  feed_markup = feed_markup + "<a class='title' href='" + entry.link + "'>" + entry.title + "</a>";
		           	  
		           	  if(feed_setting.show_content){
		           		feed_markup = feed_markup + "<br />" + date + "<span class='description'>" +
                  		content + 
                  		"<span/>";
		           	  }
		           	  
		           	  feed_markup = feed_markup + "</div>";
		           	  
		              var div = $(feed_markup);
		   	          container.append(div);
		    	    }
		      	  
		          }
		        });
		  });
	  });
  }
};

})(jQuery);


//Takes an ISO time and returns a string representing how
//long ago the date represents.
function googleFeedAPIFuzzyDate(time){
	
	var date = new Date(time),
		diff = (((new Date()).getTime() - date.getTime()) / 1000),
		day_diff = Math.floor(diff / 86400);
			
	if ( isNaN(day_diff) || day_diff < 0 || day_diff >= 31 )
		return;
			
	return day_diff == 0 && (
			diff < 60 && "just now" ||
			diff < 120 && "1 minute ago" ||
			diff < 3600 && Math.floor( diff / 60 ) + " minutes ago" ||
			diff < 7200 && "1 hour ago" ||
			diff < 86400 && Math.floor( diff / 3600 ) + " hours ago") ||
		day_diff == 1 && "Yesterday" ||
		day_diff < 7 && day_diff + " days ago" ||
		day_diff < 31 && Math.ceil( day_diff / 7 ) + " weeks ago";
}

//Takes an ISO time and returns a string representing how
//long ago the date represents.
function googleFeedAPIFormalDate(time){
	
	var date = new Date(time);
	var month = date.getMonth();
	var day = date.getDate();
	var year = date.getFullYear();
	
	var montharray=new Array("January","February","March","April","May","June",
                            "July","August","September","October","November","December");
	
	return montharray[month]+" "+day+", "+year;
}
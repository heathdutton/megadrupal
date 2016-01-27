/**
 * @file The primary javascript file for the application.
 */

/**
 * The photos array is like the model here.  It will hold all of the picture
 * information
 */
var photos = new Array();
var config = new Object();

/**
 * On Document Load...
 */
$(document).ready( function () {
    // Open up cross-domain restrictions.
    $.support.cors = true;
    $.mobile.allowCrossDomainPages = true;
    
    // Hide the message frame until we have a message.
    $('#message-frame').hide();
    
    // Assign a click handler to the take photo button.
    $('#take-photo-button').click(function () {
        takePicture(true);
    });

    // Assign a click handler to the upload photo button.
    $('#upload-photo-button').click(function () {
        takePicture(false);
    });
    
    // Initialize the config global.
    config = getConfig();
    
    // Initialize the form with any settings.
    for (i in config) {
        $('#' + i).val(config[i]);
    }

    // When the refresh button is refreshed, refresh the photo model.
    $('#view-refresh-button').click( function () {
        $('#view-photo-stream').prepend('<img src="images/ajax-loader.gif" class="image-load-spinner" />');
        getPhotos(0);
    });
    
    // When the 'more' button is clicked, 
    $('#view-more-button').click( function () {
        $(this).parent().after('<img src="images/ajax-loader.gif" class="image-load-spinner" />');
        // Gather the nids from every displayed node.
        var displayedNids = new Array();
        $('.photo-display-box').each( function () {
            displayedNids.push($(this).attr('data-nid'));
        });
        
        // Find the minimum nid that is displayed and then ask for the next set.
        var minDisplayed = Math.min.apply(Math,displayedNids);
        getPhotos(minDisplayed);
    });
    
    // Settings and handlers for swipe actions.
    $.event.special.swipe.horizontalDistanceThreshold = '200';
    $(document).bind('swiperight', function () { 
                $.mobile.changePage('#photo-upload');
    });
    $(document).bind('swipeleft', function () { 
                $.mobile.changePage('#view');
    });
    
    
    // Config button click handler.
    $('.settings-button').click(function () {
        var config = getConfig();
        config.error = 'all';
        configPopup(config);
    });
    
    // On any page transition, make sure we have the most recent form data.
    $(document).bind('pagebeforechange', function (e, data) {
        // Hide settings page so it doesn't show up during transition
        if (typeof(data.toPage) == 'object' && data.toPage.attr('id') != 'settings') {
            $('#settings').addClass('hideme');
        }
        else {
            $('#settings').removeClass('hideme');
        }
        
        // Save the form data to the local storage.
        window.localStorage.setItem('secretcode',$('#secretcode').val());
        window.localStorage.setItem('domain',$('#domain').val());
        window.localStorage.setItem('name',$('#name').val());
        
        // Hide the message frame.
        $('#message-frame').hide();
        $('#photo-preview').attr('src','images/pixgather.png');
        $('#photo-preview').removeClass('small-image');
    });
    
    // If they haven't put in any of the config data, go straight to the
    // settings form.  Otherwise, try to pull in some pictures.
    if (window.localStorage.getItem('secretcode') === null || window.localStorage.getItem('domain') === null || window.localStorage.getItem('name') === null) {
        configPopup(config);
    }
    else {
        getPhotos(0);    
    }
    
});


/**
 * Get the picture file from the camera or the photolibrary.
 * 
 * The camera parameter is true if we want to get the picture from the camera,
 * false if we want to get it from the photo library.
 * 
 */
function takePicture(camera) {
    if (camera == true) {
    }
    else {
        var source = Camera.PictureSourceType.PHOTOLIBRARY;
    }
    $('#photo-preview').attr('src','images/ajax-loader.gif').addClass('small-image');
    $('#message-frame').html('Please Wait...Uploading').show(300);
    var cameraOptions = {
        destinationType : Camera.DestinationType.DATA_URL,
        sourceType : source, 
        encodingType: Camera.EncodingType.JPEG,
        saveToPhotoAlbum: false
    };
    navigator.camera.getPicture( gotPicture, pictureFailed, cameraOptions); 
}

/**
 * The picture was successfully received.  Send the data off to the drupal site.
 * 
 * imageData is the base64 encoded image.
 * 
 */
function gotPicture(imageData) {
	
	// Check for the name setting, if not, use Anonymous.
    if (window.localStorage.getItem('name') == null) {
        var name = "Anonymous";
    }
    else {
        var name = window.localStorage.getItem('name');
    }
    
    // data will be sent off to the drupal site.
    var data = {
        'file':{
            'file':imageData,
            'filename': "test.jpg",
            'filepath':'sites/default/files/test.jpg'
        },
        'name': name,
        'secretcode': window.localStorage.getItem('secretcode')
    };
    
    // Send the data.
    var jqxhr = $.post("http://" + window.localStorage.getItem('domain') +  "/pixgather-photo-upload", data,
        function(response, status, jqXHR) {
            
            // If the response is simply WRONGCODE, the drupal module rejected the secret code.
            if (response == 'WRONGCODE') {
                setError('Error: Secret code is incorrect');
                configPopup(config);
            }
            else {
                // If we got a solid response, show a preview picture
                $('#photo-preview').attr('src',response).load(function () {
                    $('#message-frame').html('New Photo Posted!');
                    $('#photo-preview').removeClass('small-image');
                    $('#view-refresh-button').click();
                });
            } 
      });
      // If the request failed, informt the user.
      jqxhr.error(function () {
          setError("Connection Error: Check your internet connection and the 'domain ' in the settings");
      });	
       
}

/*
 * Something went wrong getting the picture.
 */
function pictureFailed() {
    setError('Sorry, something went wrong.  Try taking your picture again');
}

/*
 * 
 * Query for additional photos.  Provide the lowest nid so we can ask for photos
 * older than that.  Put them in the newPhotos array and call update photos.
 * 
 */
function getPhotos(lowest_nid) {
    var newPhotos = Array();
    var jqxhr = $.get("http://" + window.localStorage.getItem('domain') + '/pixgather-photo-view/' + lowest_nid, function (data, status, jqXHR) {
        if (data != 'NONE') {
            var nodes = eval("(" + data + ")");
            for (var nid in nodes) {

                newPhotos.push({
                    nid:nid,
                    url:nodes[nid].url,
                    author:nodes[nid].author
                });
            } 
            updatePhotos(newPhotos);
        }
    });
    
    jqxhr.error( function () {
    	for(var j in jqxhr) {
    		console.log(jqxhr[j]);
    	}
       //setError('Connection Error: check your internet connection and your \'domain\' in your settings'); 
    });
}

/* 
 * Update photos takes the new photos from the query and incorporates them into
 * the existing photos object.  It then updates the display.
 */
function updatePhotos(newPhotos) {
    $('#view-photo-stream .photo-display-box').last().addClass('last-photo');
    if (photos.length == 0) photos = newPhotos;
    else {
        newPhotos.reverse();
        for (i in newPhotos) {
            if (parseInt(newPhotos[i].nid) < parseInt(photos[0].nid)) {
                photos.unshift(newPhotos[i]);
            }
            else if (parseInt(newPhotos[i].nid)  > parseInt(photos[photos.length-1].nid)) {
                photos.push(newPhotos[i]);
            }
        }
    }
    updatePhotoDisplay();
}

// Make sure the displayed photos match the model.
function updatePhotoDisplay() {
    
    var displayedNids = new Array();
    $('.photo-display-box').each( function () {
        displayedNids.push($(this).attr('data-nid'));
    });
    
    var photoNids = new Array();
    for (i in photos) {
        photoNids.push (photos[i].nid);
    }
    
    if (displayedNids.length == 0) {
        // This is our first display.
        for (i in photos) {
            addPhotoDisplay(photos[i],'top');
        }
    }
    else {
        var maxDisplayed = Math.max.apply(Math, displayedNids);
        if (Math.max.apply(Math, photoNids) > maxDisplayed) {
            // We have new photos to display at the top.
            for (i in photos) {
                if (photos[i].nid > maxDisplayed) {
                    addPhotoDisplay(photos[i],'top');
                }
            }
        }
        var minDisplayed = Math.min.apply(Math,displayedNids);
        if (Math.min.apply(Math, photoNids) < minDisplayed ) {
            // We have new photos to display at the bottom.
            for (i in photos) {
                if (photos[i].nid < minDisplayed) {
                    addPhotoDisplay(photos[i],'bottom');
                }
            }
        }
        $('.image-load-spinner').remove();
    }
}

/*
 * Theme a single photo.
 */
function addPhotoDisplay(photo, location) {
    $('.image-load-spinner').remove();
    var content = "<div style='position:absolute;left:-99999px;' class='photo-display-box' data-nid='" + photo.nid + "'><img src='" + photo.url + "' /><div class='view-photo-name'>" + photo.author + "</div></div>";
    if (location == 'top') {
        $('#view-photo-stream').prepend(content).find('.photo-display-box img').first().load( function () {
           $(this).closest('.photo-display-box').hide(function () {
              $(this).attr('style','display:none;').show(300); 
           });
        });
    }
    else {
        $('#view-photo-stream .photo-display-box.last-photo').last().after(content).parent().find('.photo-display-box img').load( function () {
            $(this).closest('.photo-display-box').hide(function () {
               $(this).attr('style','display:none;').show(300); 
            });
        });
    }
}


/*
 * Figure out which popup to display and display it.
 */
function configPopup(config) {
        $.mobile.changePage('#settings');
}

/*
 * Load the local storage variables into a config object.
 */
function getConfig() {
    return {
            domain:window.localStorage.getItem('domain'),
            name:window.localStorage.getItem('name'),
            secretcode:window.localStorage.getItem('secretcode'),
            error:false
    };
}

/*
 * Display an error.
 */
function setError(text) {
	$('.image-load-spinner').remove();
	$('#message-content').html(text);
        $.mobile.changePage('#message'); 
        $('#message-frame').hide();
        $('#photo-preview').attr('src','images/pixgather.png').removeClass('small-image');

}

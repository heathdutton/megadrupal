/*
Copyright 2011 : Simone Gianni <simoneg@apache.org>

-- update by tcuttrissweb --
   adds in title besdie thumbs in carousel.
   adjusted css from the original to make room for this
     allows resizing
       to adjust size of the player adjust the css for:
       .youtube iframe.player width / height accordingly.

-- update by oomlaut --
   isolated function in its own scope, preventing $ collisions
   converted to jQuery plugin pattern.
      allows for chainability
   added link to YouTube Channel (optional)
   refactored callback functionality
   adjusted css:
      visual representation of currently viewing item
      

Released under The Apache License 2.0 
http://www.apache.org/licenses/LICENSE-2.0

*/
if (typeof jQuery != "undefined") {
    (function($) {
        $(document).ready(function() {
            $.fn.extend({
                youTubeChannel: function(usrArgsObj, callback) {
                    var create = {
                        feed: function() {
                            this.player(settings.videos[settings.loadItem], settings.autoplay);
                            this.carousel();
                            if (settings.channelText !== null) settings.container.append(this.channelLink);
                            if ($.isFunction(callback)) callback();
                            return true;
                            },
                        carousel: function() {
                            var scope = this;
                            var $car = $('ul.carousel', settings.container);
                            if ($car.length === 0) {
                                $car = $('<ul>', {
                                    "class": "playlist"
                                });
                                settings.container.append($car);
                            }
                            $.each(settings.videos, function(i, video) {
                                if (i < settings.showMax) {
                                    $car.append(create.thumbnail(video, i == settings.loadItem));
                                }
                            });
                            return $car;
                        },
                        thumbnail: function(video, selected) {
                            var scope = this;
                            var imgurl = video.thumbnails[0].url;
                            var img = $('img[src="' + imgurl + '"]');
                            var desc;
                            var container;
                            if (img.length !== 0) return;
                            var item = $('<li>', {
                                "class": "item",
                                click: function() {
                                    var $this = $(this);
                                    if (!$this.hasClass('nowPlaying')) {
                                        $(this).addClass('nowPlaying').siblings('.nowPlaying').removeClass('nowPlaying');
                                        scope.player(video, true);
                                    }
                                }
                            });
                            if (selected) item.addClass('nowPlaying');
                            img = $('<img>', {
                                "class": "thumbnail",
                                src: imgurl,
                                title: video.title
                            }).appendTo(item);
                            desc = $('<p>', {
                                "class": "description",
                                text: video.title
                            }).appendTo(item);
                            return item;
                        },
                        player: function(video, autoplay) {
                            var scope = this;
                            var opts = settings;
                            if (arguments.length > 1 && autoplay) {
                                opts.playopts.autoplay = 1;
                            }

                            var src = 'http://www.youtube.com/embed/' + video.id;
                            if (opts.playopts) {
                                src += '?';
                                $.each(opts.playopts, function(i, v) {
                                    src += i + '=' + v + '&';
                                })
                                src += '_a=b';
                            }

                            var ifr = $('iframe', settings.container);
                            if (ifr.length === 0) {
                                ifr = $('<iframe>', {
                                    "class": "player",
                                    scrolling: "no",
                                }).appendTo(settings.container);
                            }
                            ifr.attr('src', src);
                        },
                        channelLink: function() {
                            var scope = this;
                            var link = $('<a>', {
                                "class": "channelLink",
                                text: settings.channelText,
                                href: "http://www.youtube.com/user/" + settings.user,
                                target: "_blank"
                            });
                            return link;
                        }
                    };

                    var defaults = {
                        autoplay: false,
                        user: null,
                        container: null,
                        videos: [],
                        loadItem: 0,
                        showMax: 30,
                        channelText: null,
                        playopts: {
                            autoplay: 0,
                            egm: 1,
                            autohide: 1,
                            fs: 1,
                            showinfo: 1
                        }
                    };
                    var settings = $.extend(true, {}, defaults, usrArgsObj);

                    return this.each(function(i, v) {
                        settings.container = $(this).addClass('youtube-channel');
                        $.getJSON('http://gdata.youtube.com/feeds/users/' + settings.user + '/uploads?alt=json-in-script&format=5&callback=?', null, function(data) {
                            $.each(data.feed.entry, function(i, entry) {
                                settings.videos.push({
                                    title: entry.title.$t,
                                    id: entry.id.$t.match('[^/]*$'),
                                    thumbnails: entry.media$group.media$thumbnail
                                });
                            });
                            create.feed();
                        });
                    });
                }
            }); //end plugin
            var mobilizer_youtube_channel_username  = Drupal.settings.mobilizer_youtube_channel_username;
            $('#accountactivity').youTubeChannel({
                user: mobilizer_youtube_channel_username,//'tseries',
                showMax: 10,
                channelText: "Show all"
            }, function() {
                $('#accountactivity').find('li.item:even').css({
                    clear: "left"
                }).end().find(".channelLink").addClass('more');
            });

        }); //document.ready
    })(jQuery); //jquery anonymous function
} //endif
//]]>

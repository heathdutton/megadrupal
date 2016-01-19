
(function($) {
    $.fn.followbox = function(options) {
        var element=$(this);
        var settings = $.extend( {
            'user'   : 'twitter',
            'width'  : 292,
            'height' : 252,
            'theme'  : 'light',
            'border_color' : '#AAA',
            'bg_color' : '#fff',
            'bg_image' : '',
            'title_color' : '#3B5998',
            'total_count_color' : '#333',
            'follower_name_color' : '#BBB'
        }, options);
        //twitter user lookup
        $.ajax({
            url: 'https://api.twitter.com/1/users/lookup.json?screen_name='+settings.user+'&include_entities=true',
            dataType: 'jsonp',
            success: function(data) {
                var widget_width=settings.width-2;
                var widget_height=settings.height-2;
                var grid_container_height=settings.height-115;
                var number_images_row=parseInt(settings.width / 55);
                var number_images_col=parseInt(grid_container_height / 69)+1;
                var total_followers=number_images_row*number_images_col;
                element.html('<div class="follow_box_main" style="border: 1px solid #bbb; width: '+widget_width+'px; height: '+widget_height+'px;"><div class="follow_box_widget"><div class="follow_box"><div><div class="follow_top clearfix"><a href="http://www.twitter.com/'+settings.user+'" target="_blank"><img class="profileimage img" src="'+data[0].profile_image_url_https+'" alt="'+data[0].name+'"></a><div class="follow_action"><div class="name_block"><a href="http://www.twitter.com/'+settings.user+'" target="_blank"><span class="name titlecase">'+data[0].name.toLowerCase()+'</span> @'+data[0].screen_name+'</a></div><div class="follow_button"><iframe allowtransparency="true" frameborder="0" scrolling="no" src="//platform.twitter.com/widgets/follow_button.html?screen_name='+settings.user+'&show_count=false&show_screen_name=false" style="width:100px; height:20px;"></iframe></div></div></div><div class="connections"><span class="total"><span class="follow_box_follower_count">'+data[0].followers_count+'</span> people follow <b class="titlecase">'+data[0].name.toLowerCase()+'</b></span><div class="connections_grid clearfix" style="height:'+grid_container_height+'px;"></div></div></div><div style="height: 23px"><div class="follow_widget_footer"><div class="footer_border"><div class="clearfix uiImageBlock"><div class="footer_text"><a class="footer_text_link" target="_blank" href="http://www.devsaran.com">Twitter Social Module</a></div></div></div></div></div></div></div></div>');
                //applying dark style if theme is 'dark'
                if(settings.theme=='dark'){
                    element.find('.follow_box_main').addClass('dark');
                }
                element.find('.follow_box_follower_count').text(element.find('.follow_box_follower_count').text().replace(/(\d)(?=(\d\d\d)+(?!\d))/g, "$1,") );
                if($.browser.msie && !$.support.boxModel)
                    $('.follow_box .connections').css('padding-bottom','14px');
                if(settings.theme=='custom')
                {
                    element.find('.follow_box_main').css({
                        'border-color':settings.border_color,
                        'background-color':settings.bg_color,
                        'background-image': 'url("'+settings.bg_image+'")'
                    });
                    element.find('.follow_box a').css({
                        'color':settings.title_color
                    });
                    element.find('.follow_box .total').css({
                        'color':settings.total_count_color
                    });
                }
                $.ajax({
                    url: 'https://api.twitter.com/1/followers/ids.json?cursor=-1&screen_name='+settings.user,
                    dataType: 'jsonp',
                    success: function(followers) {
                        var idlist_string=getfollowers(total_followers,followers.ids);
                        $.ajax({
                            url: 'https://api.twitter.com/1/users/lookup.json?user_id='+idlist_string+'&include_entities=true',
                            dataType: 'jsonp',
                            success: function(follower_details) {
                                for(var i=0;i<follower_details.length;i++)
                                {
                                    var fullname=$.trim(follower_details[i].name);
                                    var namearray=fullname.split(' ');
                                    var grid_item_html='<div class="grid_item"><a href="http://twitter.com/'+follower_details[i].screen_name+'" target="_blank"><img class="img" src="'+follower_details[i].profile_image_url+'" alt=""><div class="name titlecase">'+namearray[0].toLowerCase()+'</div></a></div>';
                                    element.find('.connections_grid').append(grid_item_html);
                                }
                                if(settings.theme=='custom')
                                {
                                    element.find('.connections .connections_grid .grid_item .name').css({
                                        'color':settings.follower_name_color
                                    });
                                }
                            }
                        });
                    }
                });
            }
        });
        function getfollowers(number,followers){
            if(number>100)
                number=100;
            var idlist=new Array();
            for(var i=0;i<number;i++)
            {
                idlist.push(followers[i]);
            }
            var idlist_string=idlist.join();
            return idlist_string;
        }
    };
})(jQuery);
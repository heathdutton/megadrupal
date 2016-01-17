/**
 * dont use (function ($) {  })(jQuery);
 * otherwise the call from:
 * Drupal.ajax.prototype.commands.archibald_ajax_update_classifications
 * will not longer work
 * and the dsicipline_c reload after add button click will stop working
 */

Drupal.behaviors.archibald_curriculum_per = {
  attach: function (context, settings) {
    jQuery('.curr_per_discipline_c').empty();

    jQuery('.curr_per_education_level').each(function() {
        jQuery(this).unbind('change');
        jQuery(this).bind('change',   function () {
            archibald_curriculum_per_inst.code_level_handler();
        });
    });

    jQuery('.curr_per_discipline_a').each(function() {
        jQuery(this).unbind('change');
        jQuery(this).bind('change',   function () {
            archibald_curriculum_per_inst.code_level_handler();
        });
    });

    jQuery('.curr_per_discipline_c').each(function() {
        jQuery(this).unbind('change');
        jQuery(this).bind('change',   function () {
            archibald_curriculum_per_inst.get_default_curriculum_proposals();
            archibald_curriculum_per_inst.get_subtitles();
        });
    });

    jQuery(".curr_per_subtitles").each(function() {
        jQuery(this).unbind('change');
        jQuery(this).bind('change',   function () {
            archibald_curriculum_per_inst.process_details();
        });
    });

    // init
    var archibald_curriculum_per_inst = new archibald_curriculum_per( settings.archibald.urls.classification );

    // set all fields to zero
    archibald_curriculum_per_inst.code_level_handler();
    archibald_curriculum_per_inst.process_subtitles();
    archibald_curriculum_per_inst.process_details();
  }
};

/**
 * curriculum per handler
 * @param base_url string
 */
function archibald_curriculum_per(base_url) {
    this.base_url = base_url;

    /**
     * handler for discipline_c level "Code objectif d'apprentissage"
     */
    this.code_level_handler = function() {
        var education_level = jQuery('.curr_per_education_level').val();
        var discipline_a = jQuery('.curr_per_discipline_a').val();

        var current_disc_val = jQuery('.curr_per_discipline_c option:selected').val()+'###'+jQuery('.curr_per_discipline_c option:selected').text();

        jQuery('.curr_per_discipline_c').empty();
        var that = this;

        if (education_level && discipline_a && education_level.length>0 && discipline_a.length>0) {
            jQuery("<option/>").val("").text(' - ' + Drupal.t('Please wait') + ' - ').appendTo(".curr_per_discipline_c");

            jQuery.ajax({
                type: 'POST',
                url: this.base_url+'/getDisciplineC',
                data: {machine_name:'per',
                       session_ident:get_session_ident(),
                       education_level:education_level,
                       discipline_a:discipline_a,
                       custom_handler: 'true'
                      },
                success: function(response, status) {
                    jQuery('.curr_per_discipline_c').empty();
                    for(discipline_b in response) {
                        var optgroup = jQuery("<optgroup />").attr('label', discipline_b).appendTo(".curr_per_discipline_c");
                        for(discipline_c in response[discipline_b]) {
                            var option = jQuery("<option/>").val(response[discipline_b][discipline_c].code).text(discipline_c+' ('+response[discipline_b][discipline_c].code+')');

                            if ((option.val()+'###'+option.text())==current_disc_val) {
                                option.attr('selected',true);
                            }

                            option.appendTo(optgroup);
                        }
                    }

                    // if only the années and not the cycle was changed
                    var disc_val = jQuery('.curr_per_discipline_c').val()+'###'+jQuery('.curr_per_discipline_c').text();
                    if (disc_val==undefined || disc_val.length<1) {
                        that.current_subtitles = null;
                    }

                    that.process_subtitles();
                },
                dataType: 'json'
            });
        }
    }

    /**
     * get proposales for default curriculum
     **/
    this.get_default_curriculum_proposals = function() {
        var discipline_c = jQuery('.curr_per_discipline_c').val();
        var that = this;

        if (discipline_c.length==0) {
            return false;
        }

        if (jQuery('input#edit-proposals').size()==0) {
            return false;
        }

        var proposal_selelector = jQuery('.form-item-proposals #proposal_selelector');
        if (proposal_selelector.length<1) {
            // if it is empty, create the #proposal_selelector
            proposal_selelector = jQuery('<div id="proposal_selelector">&nbsp;</div>');
            jQuery('.form-item-proposals').append(proposal_selelector);

            // set the description layer below the proposal_selector
            var description_field = jQuery('.form-item-proposals .description');
            description_field.detach();
            jQuery('.form-item-proposals').append(description_field);
        }

        proposal_selelector.html( jQuery('<div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div></div>') );

        jQuery.ajax({
            type: 'POST',
            url: this.base_url+'/getProposals',
            data: {machine_name:'per',
                   session_ident:get_session_ident(),
                   discipline_c:discipline_c,
                   custom_handler: 'true'
                  },
            success: function(response, status) {
                var html = '';
                for(ident in response.proposals) {
                    html += '<div class="proposal_option_block"><input type="checkbox" class="proposal_option" value="'+ident+'" checked /><div>'+response.proposals[ident]+'</div></div>';
                }
                proposal_selelector.html(html);

                jQuery('.proposal_option_block input[type=checkbox]').unbind('change');
                jQuery('.proposal_option_block input[type=checkbox]').bind('change', that.process_proposals);

                that.process_proposals();
            },
            dataType: 'json'
        });

        return true;
    };

    /**
     * process propasals checboxes
     * and fill up hidden text field
     **/
    this.process_proposals = function () {
        var proposal_options = new Array();
        jQuery('.proposal_option_block input[type=checkbox]:checked').each(function () {
            proposal_options.push( jQuery(this).val() );
        });
        jQuery('input#edit-proposals').val(proposal_options.join("|$|"));
    }

    /**
     * load list of "Sous-titre"
     **/
    this.get_subtitles = function() {
        var discipline_c = jQuery('.curr_per_discipline_c').val();
        var that = this;

        if (discipline_c.length==0) {
            return false;
        }

        jQuery.ajax({
            type: 'POST',
            url: this.base_url+'/getSubtitles',
            data: {machine_name:'per',
                   session_ident:get_session_ident(),
                   discipline_c:discipline_c,
                   custom_handler: 'true'
                  },
            success: function(response, status) {
                that.current_subtitles = response.subtitle;
                that.process_subtitles();
            },
            dataType: 'json'
        });

        return true;
    };

    /**
     * @var array cache vor "Sous-titre" including details, collected with ajax based on "Code objectif d'apprentissage"
     */
    this.current_subtitles = null;

    /**
     * build up form element for "Sous-titre" based on the cache
     */
    this.process_subtitles = function( ) {
        if (this.current_subtitles==undefined || this.current_subtitles.length==0) {
            jQuery('div.form-item-subtitles').hide();

            archibald_modal_frame_heigh_auto();
            return false;
        }

        jQuery('div.form-item-subtitles').show();

        var current_subtitle_val = jQuery('.curr_per_subtitles option:selected').val()+'###'+jQuery('.curr_per_subtitles option:selected').text();

        jQuery(".curr_per_subtitles").empty();

        var subtitle = null;
        var option = null;
        for(var i in this.current_subtitles) {
            subtitle = this.current_subtitles[i];

            if (this.get_details_by_subtitle(subtitle.uuid).length>0) {
                if (subtitle.title == 'GENERAL') {
                  subtitle.title = 'Aucun';
                }
                option = jQuery("<option class='level1' />")
                    .val(subtitle.uuid)
                    .text(subtitle.title);

                if ((option.val()+'###'+option.text())==current_subtitle_val) {
                    option.attr('selected',true);
                }

                option.appendTo('.curr_per_subtitles');

                if (this.current_subtitles[i].childs && this.current_subtitles[i].childs.length>0) {
                    for(var x in this.current_subtitles[i].childs) {
                        subtitle = this.current_subtitles[i].childs[x];
                        if (this.get_details_by_subtitle(subtitle.uuid).length>0) {
                            option = jQuery("<option class='level2' />")
                                .val(subtitle.uuid)
                                .text(subtitle.title);

                            if ((option.val()+'###'+option.text())==current_subtitle_val) {
                                option.attr('selected',true);
                            }

                            option.appendTo('.curr_per_subtitles');
                        }
                    }
                }
            }
        }

        if (jQuery('.curr_per_subtitles option').size()==0) {
            jQuery('div.form-item-subtitles').hide();
        }

        if (jQuery('.curr_per_subtitles option').size()==1) {
            // select if it only on element exist
            jQuery('.curr_per_subtitles option').attr('selected', true);
        }
        this.process_details();

        archibald_modal_frame_heigh_auto();
        return true;
    };

    /**
     * get a list of all matching "texte progression des apprentissages"
     * by given uuid
     */
    this.get_details_by_subtitle = function(uuid) {
        var details = [];

        var education_level = '';
        if (jQuery('.curr_per_education_level option:selected').text().substring(0, 2)=='- ') {
            // it`s a second level (années)
            education_level = jQuery('.curr_per_education_level').val();
        }

        var subtitle=null, detail=null, first_level_match=false, d=0;
        for(var i in this.current_subtitles) {
            subtitle = this.current_subtitles[i];

            first_level_match = false;
            if (subtitle.uuid==uuid) {
                // ok is the searched element
                if (subtitle.details && subtitle.details.length>0) {
                    for(d in subtitle.details) {
                        detail = subtitle.details[d];

                        if (education_level.length==0 || detail.school_year.length==0 || this.in_array(education_level, detail.school_year)) {
                            details.push(detail);
                        }
                    }
                }
                first_level_match = true;
            }

            if (this.current_subtitles[i].childs && this.current_subtitles[i].childs.length>0) {
                for(var x in this.current_subtitles[i].childs) {
                    subtitle = this.current_subtitles[i].childs[x];

                    if (subtitle.uuid==uuid || first_level_match==true) {
                        // ok is the searched element
                        if (subtitle.details && subtitle.details.length>0) {
                            for(d in subtitle.details) {
                                detail = subtitle.details[d];

                                if (education_level.length==0 || detail.school_year.length==0 || this.in_array(education_level, detail.school_year)) {
                                    details.push(detail);
                                }
                            }
                        }
                    }
                }
            }
        }

        return details;


    };

    /**
     * build up form element for "texte progression des apprentissages" based on the cache
     */
    this.process_details = function () {
       var subtitle_uuid = jQuery('.curr_per_subtitles').val();
       var details = this.get_details_by_subtitle(subtitle_uuid);

       jQuery('div.form-item-details').hide();

       if (this.current_subtitles==undefined || this.current_subtitles.length==0 || subtitle_uuid==undefined || subtitle_uuid.length==0 || details.length==0) {
            jQuery('div.form-item-details').hide();

            archibald_modal_frame_heigh_auto();
            return false;
        }

        jQuery('div.form-item-details').show();

        var details_selelector = jQuery('.form-item-details #details_selelector');
        if (details_selelector.length<1) {
            // if it is empty, create the #proposal_selelector
            details_selelector = jQuery('<div id="details_selelector">&nbsp;</div>');
            jQuery('.form-item-details').append(details_selelector);

            // set the description layer below the proposal_selector
            var description_field = jQuery('.form-item-details .description');
            description_field.detach();
            jQuery('.form-item-details').append(description_field);
        }

        var html = '';
        for(var i in details) {
            html += '<div class="details_option_block"><input type="checkbox" class="proposal_option" value="'+details[i].url_part+'" /><div>'+details[i].text+'</div></div>';
        }
        details_selelector.html(html);

        jQuery('.details_option_block input[type=checkbox]').unbind('change');
        jQuery('.details_option_block input[type=checkbox]').bind('change',   function () {
            var details_options = new Array();
            jQuery('.details_option_block input[type=checkbox]:checked').each(function () {
                details_options.push( jQuery(this).val() );
            });
            jQuery('input#edit-details').val(details_options.join("|$|"));
        });

        archibald_modal_frame_heigh_auto();
        return true;
    }

    /**
     * replacement for php in_array()
     * @param item string
     * @param arr array
     * @return boolean
     */
    this.in_array = function(item,arr) {
        for(p=0;p<arr.length;p++) if (item == arr[p]) {
            return true;
        }

        return false;
    }
}
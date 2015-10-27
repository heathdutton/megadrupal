/**
 * Update, show, hide fields
 */

(function ($) {

Drupal.behaviors.validateFields = {
  attach: function (context) {
    //price negotiable context
    $(".form-item-field-immo-price-und-form-vb", context).click(function () {
       var value = $("input[name='field_immo_price[und][form][vb]']:checked").val();
       if(value === "1"){
         $("input[name='field_immo_price[und][form][kaufpreis]']").val("0");
         $("input[name='field_immo_price[und][form][kaufpreis]']").closest(".price_left").addClass("price_none");
         $("input[name='field_immo_price[und][form][auf_anfrage]']").closest(".price_left").addClass("price_none");
       }
       if(value === "0"){
         $("input[name='field_immo_price[und][form][kaufpreis]']").closest(".price_left").removeClass("price_none");
         $("input[name='field_immo_price[und][form][auf_anfrage]']").closest(".price_left").removeClass("price_none");
         $("input[name='field_immo_price[und][form][richtpreis]']").val("0");
       }
    });
    //Basis-Basics
    $("#edit-field-nutzungsart-und",context).click(function (){
       //wohnen
       if($("#edit-field-nutzungsart-und").val() === "WOHNEN"){
         $("#edit-field-gewerbliche-nutzung").closest(".collapse-processed").removeClass("price_none"); 
       }
       if($("#edit-field-nutzungsart-und") .val() !== "WOHNEN"){
         $("#edit-field-gewerbliche-nutzung").closest(".collapse-processed").addClass("price_none");
         $("input[name ='field_branchen[und][0][value]']").val('');
         $("input[name ='field_gewerbliche_nutzung[und]']").attr('checked',false);
       }
       //wohnen oder waz
       if($("#edit-field-nutzungsart-und").val() !== "WOHNEN" && 
               $("#edit-field-nutzungsart-und").val() !== "WAZ"  ){
         $(".group-hab").addClass("price_none");
         $(".group-commercial").removeClass("price_none");
         $(".group-habitation").addClass("price_none");
       }    
       if($("#edit-field-nutzungsart-und").val() === "WOHNEN" || 
               $("#edit-field-nutzungsart-und").val() === "WAZ"){
         $(".group-hab").removeClass("price_none");
         $(".group-habitation").removeClass("price_none");
         $(".group-commercial").addClass("price_none");
         $("#edit-field-als-ferien").removeClass("price_none");
         $(".group-rent-duration").removeClass("price_none");
       }
       if($("#edit-field-nutzungsart-und").val() !== "WAZ"){
         $("#edit-field-als-ferien").addClass("price_none");
         $(".group-rent-duration").addClass("price_none");
       }
       if($("#edit-field-nutzungsart-und").val() === "_none"){
         $.each($('#edit-group_object_type .field-group-fieldset'), function(index, value) { 
           $(this).addClass("price_none");
         });
         var schraeg_haus = '<div class="schraeg">'+ Drupal.t('Please choose "Basics"->"type of use" and "Basics"->"type of marketing" first.')+'</div>';
         $("#edit-group_object_type").append(schraeg_haus);
       }
       if($("#edit-field-nutzungsart-und").val() !== "_none"){
         $("#edit-group_object_type .schraeg").hide();                  
         $(".group-land-plot").removeClass("price_none");
         $(".group-park-prop").removeClass("price_none"); 
       }
       if($("#edit-field-nutzungsart-und").val() === "ANLAGE"){
         $(".group-habitation").removeClass("price_none");
       }
    });
    //Basis-Objekt
    $("#edit-field-haustyp-und").click(function (){
        if($("#edit-field-haustyp-und").val() !== "_none"){
          $(".field-name-field-wohnungtyp").addClass("price_none");
          $(".field-name-field-zimmertyp").addClass("price_none");
          //first wildcard for all fields starting with field_zimmertyp
          $("input[name^='field_zimmertyp']").val("");
          $(".group-land-plot").addClass("price_none");
          $(".group-park-prop").addClass("price_none");
          $(".group-grundstueck").addClass("price_none");
          $(".group-commercial").addClass("price_none");
        }
        if($("#edit-field-haustyp-und").val() === "_none"){
          $(".field-name-field-wohnungtyp").removeClass("price_none");
          $(".field-name-field-zimmertyp").removeClass("price_none");
          $(".group-land-plot").removeClass("price_none");
          $(".group-park-prop").removeClass("price_none");
          $(".group-grundstueck").removeClass("price_none");
        }
        if($("#edit-field-haustyp-und").val() === "_none" && 
                $("#edit-field-nutzungsart-und").val() === "ANLAGE"){
          $(".group-commercial").removeClass("price_none");
        }
    });
    $("#edit-field-wohnungtyp-und").click(function (){
       if($("#edit-field-wohnungtyp-und").val() !== "_none"){
          $(".field-name-field-haustyp").addClass("price_none");
          $(".field-name-field-zimmertyp").addClass("price_none");
          //first wildcard for all fields starting with field_zimmertyp
          $("input[name^='field_zimmertyp']").val("");
          $(".group-land-plot").addClass("price_none");
          $(".group-park-prop").addClass("price_none");
          $(".group-grundstueck").addClass("price_none");
        }
        if($("#edit-field-wohnungtyp-und").val() === "_none"){
          $(".field-name-field-haustyp").removeClass("price_none");
          $(".field-name-field-zimmertyp").removeClass("price_none");
          $(".group-land-plot").removeClass("price_none");
          $(".group-park-prop").removeClass("price_none");
          $(".group-grundstueck").removeClass("price_none");
            }
        });
    $("#edit-field-parken-typ-und").click(function (){
      if($("#edit-field-parken-typ-und").val() !== "_none"){
        $(".field-name-field-haustyp").addClass("opacity");
        $("#edit-field-haustyp-und").prop("disabled",true);
        $(".field-name-field-zimmertyp").addClass("opacity");
        $("input[name^='field_zimmertyp']").prop("disabled",true);
        $(".field-name-field-wohnungtyp").addClass("opacity");
        $("#edit-field-wohnungtyp-und").prop("disabled",true);
        $(".field-name-field-wbs-sozialwohnung").addClass("opacity");
        $("input[name='field_wbs_sozialwohnung[und]']").prop("checked",false);
        $(".field-name-field-flurstueck").addClass("opacity");
        $(".field-name-field-flur").addClass("opacity");
        $(".field-name-field-gemarkung").addClass("opacity");
        //first wildcard for all fields starting with field_zimmertyp
        $("input[name^='field_zimmertyp']").val("");
        $("input[name^='field_flur']").val("");
        $("input[name^='field_gemarkung']").val("");
        $("#edit-field-grundstueck-und").prop("disabled",true);
        $("#edit-field-grundstueck-und").addClass("opacity");
        $(".group-grundstueck").addClass("price_none");
      }
      if($("#edit-field-parken-typ-und").val() === "_none"){
        $(".field-name-field-haustyp").removeClass("opacity");
        $("#edit-field-haustyp-und").prop("disabled",false);
        $(".field-name-field-wohnungtyp").removeClass("opacity");
        $("#edit-field-wohnungtyp-und").prop("disabled",false);
        $(".field-name-field-wbs-sozialwohnung").removeClass("opacity");
        $(".field-name-field-flurstueck").removeClass("opacity");
        $(".field-name-field-flur").removeClass("opacity");
        $(".field-name-field-gemarkung").removeClass("opacity");
        $(".field-name-field-zimmertyp").removeClass("opacity");
        $("input[name^='field_zimmertyp']").prop("disabled",false);
        $("#edit-field-grundstueck-und").prop("disabled",false);
        $("#edit-field-grundstueck-und").removeClass("opacity");
        $(".group-grundstueck").removeClass("price_none");
      }
    });
    $("#edit-field-grundstueck-und").click(function (){
      if($("#edit-field-grundstueck-und").val() !== "_none"){
        $(".field-name-field-haustyp").addClass("opacity");
        $("#edit-field-haustyp-und").prop("disabled",true);
        $(".field-name-field-zimmertyp").addClass("opacity");
        $("input[name^='field_zimmertyp']").prop("disabled",true);
        $("input[name^='field_zimmertyp']").val("");
        $(".field-name-field-wohnungtyp").addClass("opacity");
        $("#edit-field-wohnungtyp-und").prop("disabled",true);
        $(".field-name-field-wbs-sozialwohnung").addClass("opacity");
        $("input[name='field_wbs_sozialwohnung[und]']").prop("checked",false);
        $(".group-park-prop").addClass("price_none");
        $(".group-hab-gen").addClass("price_none");
        $("#edit-field-bebaubar-attr").removeClass("price_none");
        $("#edit-field-umf-erschl-attr").removeClass("price_none");
        $("#edit-field-erschl-attr").removeClass("price_none");
        $("#edit-field-zustand-art").addClass("price_none");
        $("#edit-field-baujahr").addClass("price_none");
        $("#edit-field-bauzone").addClass("price_none");
        $("#edit-field-letztemodernisierung").addClass("price_none");
        $("#edit-field-gebiete").addClass("price_none");
        $("#edit-field-altlasten").removeClass("price_none");
        $("#edit-field-dachform").addClass("price_none");
        $("#edit-field-bauweise").addClass("price_none");
        $("#edit-field-ausbaustufe").addClass("price_none");
        $("#edit-field-energietyp").addClass("price_none");
        $("#edit-field-fenster").addClass("price_none");
        $("#edit-field-etage").addClass("price_none");
        $("#edit-field-anzahl-etagen").addClass("price_none");
        $("#edit-field-lage-im-bau").addClass("price_none");
        $("#edit-field-alter-attr").addClass("price_none");
        $("#edit-field-zulieferung").addClass("price_none");
        $("#edit-field-blick").addClass("price_none");
        $("#edit-field-beziehbar-ab").addClass("price_none"); 
        $(".group-beziehbar").addClass("price_none");
        $.each($(".group-commercial select[id^=edit-field]"), function(index, value){
          $(this).parents().eq(1).siblings(".field-type-list-text").addClass("price_none");      
       });
       $(".group-basics-additional .fieldset-wrapper").addClass("price_none");
       $("#edit-field-flaechenangaben-und-form li.vertical-tab-button.last").addClass("price_none");
      }
      if($("#edit-field-grundstueck-und").val() === "_none"){
        $(".field-name-field-haustyp").removeClass("opacity");
        $("#edit-field-haustyp-und").prop("disabled",false);
        $(".field-name-field-zimmertyp").removeClass("opacity");
        $("input[name^='field_zimmertyp']").prop("disabled",false);
        $(".field-name-field-wohnungtyp").removeClass("opacity");
        $("#edit-field-wohnungtyp-und").prop("disabled",false);
        $(".field-name-field-wbs-sozialwohnung").removeClass("opacity");
        $(".group-park-prop").removeClass("price_none");
        $(".group-hab-gen").removeClass("price_none");
        $("#edit-field-bebaubar-attr").addClass("price_none");
        $("#edit-field-umf-erschl-attr").addClass("price_none");
        $("#edit-field-erschl-attr").addClass("price_none");
        $("#edit-field-zustand-art").removeClass("price_none");
        $("#edit-field-baujahr").removeClass("price_none");
        $("#edit-field-bauzone").removeClass("price_none");
        $("#edit-field-letztemodernisierung").removeClass("price_none");
        $("#edit-field-gebiete").removeClass("price_none");
        $("#edit-field-altlasten").addClass("price_none");
        $("#edit-field-dachform").removeClass("price_none");
        $("#edit-field-bauweise").removeClass("price_none");
        $("#edit-field-ausbaustufe").removeClass("price_none");
        $("#edit-field-energietyp").removeClass("price_none");
        $("#edit-field-fenster").removeClass("price_none");
        $("#edit-field-etage").removeClass("price_none");
        $("#edit-field-anzahl-etagen").removeClass("price_none");
        $("#edit-field-lage-im-bau").removeClass("price_none");
        $("#edit-field-alter-attr").removeClass("price_none");
        $("#edit-field-zulieferung").removeClass("price_none");
        $("#edit-field-blick").removeClass("price_none");
        $("#edit-field-beziehbar-ab").removeClass("price_none");
        $(".group-beziehbar").removeClass("price_none");
        $.each($(".group-commercial select[id^=edit-field]"), function(index, value){
         $(this).parents().eq(1).siblings(".field-type-list-text").removeClass("price_none");        
       });
        $(".group-basics-additional .fieldset-wrapper").removeClass("price_none");
        $("#edit-field-flaechenangaben-und-form li.vertical-tab-button.last").removeClass("price_none");
      }
    });
    $("#edit-field-vermarktungsart-und").click(function (){
      if($("#edit-field-vermarktungsart-und").val() !== "ERB_PACHT"){
        $(".field-name-field-laufzeit").addClass("price_none");
        $("input[name='field_laufzeit[und][0][value]']").val("");
      }
      if($("#edit-field-vermarktungsart-und").val() === "ERB_PACHT"){
        $(".field-name-field-laufzeit").removeClass("price_none");   
      }
    });
    $(".group-commercial select[id^=edit-field]").on('click.hideOthers', function(e){
      if($(e.target).val() !== "_none"){        
        $(this).parents().eq(1).siblings(".field-type-list-text").addClass("price_none");
        $(".group-land-plot").addClass("price_none");
        $(".group-park-prop").addClass("price_none");
        $(".field-name-field-haustyp").addClass("opacity");
        $("#edit-field-haustyp-und").prop("disabled",true);
        $(".field-name-field-zimmertyp").addClass("opacity");
        $("input[name^='field_zimmertyp']").prop("disabled",true);
        $("input[name^='field_zimmertyp']").val("");
        $(".field-name-field-wohnungtyp").addClass("opacity");
        $("#edit-field-wohnungtyp-und").prop("disabled",true);
        $(".field-name-field-wbs-sozialwohnung").addClass("opacity");
        $("input[name='field_wbs_sozialwohnung[und]']").prop("checked",false);
      }
      if($(e.target).val() === "_none"){        
        $(this).parents().eq(1).siblings(".field-type-list-text").removeClass("price_none");
        $(".group-land-plot").removeClass("price_none");
        $(".group-park-prop").removeClass("price_none");
        $(".field-name-field-haustyp").removeClass("opacity");
        $("#edit-field-haustyp-und").prop("disabled",false);
        $(".field-name-field-zimmertyp").removeClass("opacity");
        $("input[name^='field_zimmertyp']").prop("disabled",false);
        $(".field-name-field-wohnungtyp").removeClass("opacity");
        $("#edit-field-wohnungtyp-und").prop("disabled",false);
        $(".field-name-field-wbs-sozialwohnung").removeClass("opacity");
      }
    });
    
    //because drupal.behaviors works like document.ready:
    var value = $("input[name='field_immo_price[und][form][vb]']:checked").val();
    if(value === "1"){
      $("input[name='field_immo_price[und][form][kaufpreis]']").closest(".price_left").addClass("price_none");
      $("input[name='field_immo_price[und][form][auf_anfrage]']").closest(".price_left").addClass("price_none");
    }
    if(value === "0"){
      $("input[name='field_immo_price[und][form][kaufpreis]']").closest(".price_left").removeClass("price_none");
      $("input[name='field_immo_price[und][form][auf_anfrage]']").closest(".price_left").removeClass("price_none");
    }
    if($("#edit-field-gewerbliche-nutzung").length){
      if($("#edit-field-nutzungsart-und").val() === "WOHNEN"){
        $("#edit-field-gewerbliche-nutzung").closest(".group-com-addition").removeClass("price_none"); 
      }
      if($("#edit-field-nutzungsart-und").val() !== "WOHNEN"){
        $("#edit-field-gewerbliche-nutzung").closest(".group-com-addition").addClass("price_none");
        $("input[name ='field_branchen[und][0][value]']").val('');
        $("input[name ='field_gewerbliche_nutzung[und]']").attr('checked',false);
      }
    }
    //value field_nutzungsart:
    if($("#edit-field-nutzungsart-und").val() === "_none" && 
            $("#edit-field-vermarktungsart-und").val() === "_none"){
      var schraeg_haus = '<div class="schraeg">'+ Drupal.t('Please choose "Basics"->"type of use" and "Basics"->"type of marketing" first.')+'</div>';
      $.each($('#edit-group_object_type .field-group-fieldset'), function(index, value) { 
          $(this).addClass("price_none");
      });
      $("#edit-group_object_type").append(schraeg_haus);
      $("#edit-field-als-ferien").addClass("price_none");
      $(".group-rent-duration").addClass("price_none");
    }
    if($("#edit-field-nutzungsart-und").val() !== "WOHNEN" && 
      $("#edit-field-nutzungsart-und").val() !== "WAZ"  ){
      $(".group-hab").addClass("price_none");      
      $(".group-habitation").addClass("price_none");
    }    
    if($("#edit-field-nutzungsart-und").val() === "WOHNEN"){
      $(".group-hab").removeClass("price_none");
      $(".group-habitation").removeClass("price_none");
      $(".group-commercial").addClass("price_none");
      $("#edit-field-als-ferien").addClass("price_none");
      $(".group-rent-duration").addClass("price_none");
    }
    if($("#edit-field-nutzungsart-und").val() === "WAZ"){
      $(".group-hab").removeClass("price_none");
      $(".group-habitation").removeClass("price_none");
      $(".group-commercial").addClass("price_none");
      $("#edit-field-als-ferien").removeClass("price_none");
      $(".group-rent-duration").removeClass("price_none");
     }
    if($("#edit-field-nutzungsart-und").val() === "ANLAGE" ||
             $("#edit-field-nutzungsart-und").val() === "GEWERBE"){
      $("#edit-field-als-ferien").addClass("price_none");
      $(".group-rent-duration").addClass("price_none");
      $(".group-commercial").removeClass("price_none");
     }
    if($("#edit-field-nutzungsart-und").val() === "ANLAGE"){
       $(".group-habitation").removeClass("price_none");
      }
      
    //field_haustyp  
    if($("#edit-field-haustyp-und").val() !== "_none"){
       $(".field-name-field-wohnungtyp").addClass("price_none");
       $(".field-name-field-zimmertyp").addClass("price_none");
       $(".group-land-plot").addClass("price_none");
       $(".group-park-prop").addClass("price_none");
       $(".group-commercial").addClass("price_none");
       $(".group-grundstueck").addClass("price_none");
      }
     if($("#edit-field-wohnungtyp-und").val() !== "_none"){
       $(".field-name-field-haustyp").addClass("price_none");
       $(".field-name-field-zimmertyp").addClass("price_none");
       $(".group-land-plot").addClass("price_none");
       $(".group-park-prop").addClass("price_none");
       $(".group-grundstueck").addClass("price_none");
      }
      if($("#edit-field-parken-typ-und").val() !== "_none"){
       $(".field-name-field-haustyp").addClass("opacity");
       $("#edit-field-haustyp-und").prop("disabled",true);
       $(".field-name-field-zimmertyp").addClass("opacity");
       $("input[name^='field_zimmertyp']").prop("disabled",true);
       $(".field-name-field-wohnungtyp").addClass("opacity");
       $("#edit-field-wohnungtyp-und").prop("disabled",true);
       $(".field-name-field-wbs-sozialwohnung").addClass("opacity");
       $("input[name='field_wbs_sozialwohnung[und]']").prop("checked",false);
       $(".field-name-field-flurstueck").addClass("opacity");
       $(".field-name-field-flur").addClass("opacity");
       $(".field-name-field-gemarkung").addClass("opacity");
       //first wildcard for all fields starting with field_zimmertyp
       $("input[name^='field_zimmertyp']").val("");
       $("input[name^='field_flur']").val("");
       $("input[name^='field_gemarkung']").val("");
       $("#edit-field-grundstueck-und").prop("disabled",true);
       $("#edit-field-grundstueck-und").addClass("opacity");
       $(".group-grundstueck").addClass("price_none");       
     }
     if($("#edit-field-grundstueck-und").val() !== "_none"){
       $(".field-name-field-haustyp").addClass("opacity");
       $("#edit-field-haustyp-und").prop("disabled",true);
       $(".field-name-field-zimmertyp").addClass("opacity");
       $("input[name^='field_zimmertyp']").prop("disabled",true);
       $(".field-name-field-wohnungtyp").addClass("opacity");
       $("#edit-field-wohnungtyp-und").prop("disabled",true);
       $(".field-name-field-wbs-sozialwohnung").addClass("opacity");
       $("input[name='field_wbs_sozialwohnung[und]']").prop("checked",false);
       $(".group-park-prop").addClass("price_none");
       $(".group-hab-gen").addClass("price_none");
       $("#edit-field-bebaubar-attr").removeClass("price_none");
       $("#edit-field-umf-erschl-attr").removeClass("price_none");
       $("#edit-field-erschl-attr").removeClass("price_none");
       $("#edit-field-zustand-art").addClass("price_none");
       $("#edit-field-baujahr").addClass("price_none");
       $("#edit-field-bauzone").addClass("price_none");
       $("#edit-field-letztemodernisierung").addClass("price_none");
       $("#edit-field-gebiete").addClass("price_none");
       $("#edit-field-altlasten").addClass("price_none");
       $("#edit-field-dachform").addClass("price_none");
       $("#edit-field-bauweise").addClass("price_none");
       $("#edit-field-ausbaustufe").addClass("price_none");
       $("#edit-field-energietyp").addClass("price_none");
       $("#edit-field-fenster").addClass("price_none");
       $("#edit-field-etage").addClass("price_none");
       $("#edit-field-anzahl-etagen").addClass("price_none");
       $("#edit-field-lage-im-bau").addClass("price_none");
       $("#edit-field-alter-attr").addClass("price_none");
       $("#edit-field-zulieferung").addClass("price_none");
       $("#edit-field-blick").addClass("price_none");
       $("#edit-field-beziehbar-ab").addClass("price_none");
       $(".group-beziehbar").addClass("price_none");
       $.each($(".group-commercial select[id^=edit-field]"), function(index, value){        
         $(this).parents().eq(1).siblings(".field-type-list-text").addClass("price_none");       
       });
       $(".group-basics-additional .fieldset-wrapper").addClass("price_none");
       $("#edit-field-flaechenangaben-und-form-area-basis").addClass("price_none");
     }
     
     if($("#edit-field-grundstueck-und").val() === "_none"){
       $("#edit-field-bebaubar-attr").addClass("price_none");
       $("#edit-field-umf-erschl-attr").addClass("price_none");
       $("#edit-field-erschl-attr").addClass("price_none");
       $("#edit-field-zustand-art").removeClass("price_none");
       $("#edit-field-baujahr").removeClass("price_none");
       $("#edit-field-bauzone").removeClass("price_none");
       $("#edit-field-letztemodernisierung").removeClass("price_none");
       $("#edit-field-gebiete").removeClass("price_none");
       $("#edit-field-altlasten").removeClass("price_none");
       $("#edit-field-dachform").removeClass("price_none");
       $("#edit-field-bauweise").removeClass("price_none");
       $("#edit-field-ausbaustufe").removeClass("price_none");
       $("#edit-field-energietyp").removeClass("price_none");
       $("#edit-field-fenster").removeClass("price_none");
       $("#edit-field-etage").removeClass("price_none");
       $("#edit-field-anzahl-etagen").removeClass("price_none");
       $("#edit-field-lage-im-bau").removeClass("price_none");
       $("#edit-field-alter-attr").removeClass("price_none");
       $("#edit-field-zulieferung").removeClass("price_none");
       $("#edit-field-blick").removeClass("price_none");
       $("#edit-field-beziehbar-ab").removeClass("price_none");
       $(".group-beziehbar").removeClass("price_none");
       $.each($(".group-commercial select[id^=edit-field]"), function(index, value){        
         $(this).parents().eq(1).siblings(".field-type-list-text").removeClass("price_none");       
       });
       $(".group-basics-additional .fieldset-wrapper").removeClass("price_none");
       $("#edit-field-flaechenangaben-und-form-area-basis").removeClass("price_none");
     }
     if($("#edit-field-vermarktungsart-und").val() !== "ERB_PACHT"){
       $(".field-name-field-laufzeit").addClass("price_none");   
     }
     
     $.each($(".group-commercial select[id^=edit-field]"), function(index, value){
      if($(this).val() !== "_none"){ 
        $(this).parents().eq(1).siblings(".field-type-list-text").addClass("price_none");
        $(".group-land-plot").addClass("price_none");
        $(".group-park-prop").addClass("price_none");
        $(".field-name-field-haustyp").addClass("opacity");
        $("#edit-field-haustyp-und").prop("disabled",true);
        $(".field-name-field-zimmertyp").addClass("opacity");
        $("input[name^='field_zimmertyp']").prop("disabled",true);
        $("input[name^='field_zimmertyp']").val("");
        $(".field-name-field-wohnungtyp").addClass("opacity");
        $("#edit-field-wohnungtyp-und").prop("disabled",true);
        $(".field-name-field-wbs-sozialwohnung").addClass("opacity");
        $("input[name='field_wbs_sozialwohnung[und]']").prop("checked",false);
      }
    });
  } 
 };

})(jQuery);

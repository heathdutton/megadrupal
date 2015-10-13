// $Id: jquery.pagestyle.js,v 1.4 2009/12/24 16:49:09 christianzwahlen Exp $

(function($) {
  $(document).ready(function(){
    $("a.ps_white_black").attr({ href: "#" });
    $("a.ps_black_white").attr({ href: "#" });
    $("a.ps_yellow_blue").attr({ href: "#" });
    $("a.ps_standard").attr({ href: "#" });
    function PSremoveBC(){
      $("body").removeClass("pagestyle_black_white");
      $("body").removeClass("pagestyle_white_black");
      $("body").removeClass("pagestyle_yellow_blue");
      $("body").removeClass("pagestyle_standard");
      $("link.ps_white_black").attr({ rel: "alternate stylesheet" });
      $("link.ps_black_white").attr({ rel: "alternate stylesheet" });
      $("link.ps_yellow_blue").attr({ rel: "alternate stylesheet" });
      $("link.ps_standard").attr({ rel: "alternate stylesheet" });
    };
    function editLink(link_href){
      var ps_val = $("#edit-pagestyle-select").val();
      if (link_href = "standard") {
        $("link.ps_" + pagestyleCurrent).attr({ href: pagestylePath + "/css/style_standard.css" });
      }
      else if (link_href = "current") {
        $("link.ps_" + pagestyleCurrent).attr({ href: pagestylePath + "/css/style_" + pagestyleCurrent + ".css" });
      }
      $("link.ps_" + ps_val).attr({ href: pagestylePath + "/css/style_" + ps_val + ".css" });
    }
    $("a.ps_black_white[href=#]").click(
      function(){
        PSremoveBC();
        editLink(link_href = "current");
        $.cookie('pagestyle', "black_white", { expires: pagestyleCookieExpires, path: pagestyleCookieDomain});
        $("#pagestyle_current").empty();
        $("#pagestyle_current").append(Drupal.t('Black') + '/'+ Drupal.t('White'));
        $("#pagestyle_current").attr({ title: Drupal.t('Current Style') + ": " + Drupal.t('Black') + '/'+ Drupal.t('White')});
        $("body").addClass("pagestyle_black_white");
        return false;
      }
    );
    $("a.ps_white_black[href=#]").click(
    function (){
        PSremoveBC();
        editLink(link_href = "current");
        $.cookie('pagestyle', "white_black", { expires: pagestyleCookieExpires, path: pagestyleCookieDomain});
        $("#pagestyle_current").empty();
        $("#pagestyle_current").append(Drupal.t('White') + '/' + Drupal.t('Black'));
        $("#pagestyle_current").attr({ title: Drupal.t('Current Style') + ": " + Drupal.t('White') + '/'+ Drupal.t('Black')});
        $("body").addClass("pagestyle_white_black");
        return false;
      }
    );
    $("a.ps_yellow_blue[href=#]").click(
      function(){
        PSremoveBC();
        editLink(link_href = "current");
        $.cookie('pagestyle', "yellow_blue", { expires: pagestyleCookieExpires, path: pagestyleCookieDomain});
        $("#pagestyle_current").empty();
        $("#pagestyle_current").append(Drupal.t('Yellow') + '/' + Drupal.t('Blue'));
        $("#pagestyle_current").attr({ title: Drupal.t('Current Style') + ": " + Drupal.t('Yellow') + '/'+ Drupal.t('Blue')});
        $("body").addClass("pagestyle_yellow_blue");
        return false;
      }
    );
    $("a.ps_standard[href=#]").click(
      function(){
        PSremoveBC();
        editLink(link_href = "standard");
        $.cookie('pagestyle', "standard", { expires: pagestyleCookieExpires, path: pagestyleCookieDomain});
        $("#pagestyle_current").empty();
        $("#pagestyle_current").append(Drupal.t('Standard'));
        $("#pagestyle_current").attr({ title: Drupal.t('Current Style') + ": " + Drupal.t('Standard')});
        $("body").addClass("pagestyle_standard");
        return false;
      }
    );
    function pagestyleVals() {
      var ps_val = $("#edit-pagestyle-select").val();
        PSremoveBC();
        editLink(link_href = "standard");
        $.cookie('pagestyle', ps_val, { expires: pagestyleCookieExpires, path: pagestyleCookieDomain});
        $("body").addClass("pagestyle_" + ps_val);
        $("select.pagestyle option:selected").each(function () {
          $("#pagestyle_current").empty();
          $("#pagestyle_current").append( $(this).text() );
          }
        );
        $("body").addClass('pagestyle_' + ps_val);
        $("link.ps_" + ps_val).attr({ rel: "stylesheet" });
    }
    $("#edit-pagestyle-select").change(pagestyleVals);
    $("#edit-pagestyle-submit").hide();
    $("img.ps_rollover").hover(
      function(){
        if($(this).attr("src").indexOf("16_hover") == -1) {
          var newSrc = $(this).attr("src").replace("16.gif","16_hover.gif#hover");
          $(this).attr("src",newSrc);
        }
      },
      function(){
        if($(this).attr("src").indexOf("16_hover.gif#hover") != -1) {
          var oldSrc = $(this).attr("src").replace("16_hover.gif#hover","16.gif");
          $(this).attr("src",oldSrc);
        }
        else if($(this).attr("src").indexOf("16_focus.gif#focus") != -1) {
          var oldSrc = $(this).attr("src").replace("16_focus.gif#focus","16.gif");
          $(this).attr("src",oldSrc);
        }
      }
    );
    $("a.ps_rollover").focus(
      function(){
        var tsIMG = $(this).children("img");
        if($(tsIMG).attr("src").indexOf("16_hover.gif#hover") != -1) {
          var newSrc = $(tsIMG).attr("src").replace("16_hover.gif#hover","16_focus.gif#focus");
          $(tsIMG).attr("src",newSrc);
        }
      }
    );
    function pagestyleFW() {
      var ps_fw_bw = $("#edit-pagestyle-fontweight-black-white").val();
      var ps_fw_wb = $("#edit-pagestyle-fontweight-white-black").val();
      var ps_fw_yb = $("#edit-pagestyle-fontweight-yellow-blue").val();
      var ps_fw_s = $("#edit-pagestyle-fontweight-standard").val();
      $("div.form-item-pagestyle-fontweight-black-white label").css({"font-weight": ps_fw_bw});
      $("div.form-item-pagestyle-fontweight-white-black label").css({"font-weight": ps_fw_wb});
      $("div.form-item-pagestyle-fontweight-yellow-blue label").css({"font-weight": ps_fw_yb});
      $("div.form-item-pagestyle-fontweight-standard label").css({"font-weight": ps_fw_s});
    }
    $("#edit-pagestyle-fontweight-black-white").change(pagestyleFW);
    $("#edit-pagestyle-fontweight-white-black").change(pagestyleFW);
    $("#edit-pagestyle-fontweight-yellow-blue").change(pagestyleFW);
    $("#edit-pagestyle-fontweight-standard").change(pagestyleFW);
  });
})(jQuery);

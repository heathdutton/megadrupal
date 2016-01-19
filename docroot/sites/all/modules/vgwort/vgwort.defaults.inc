<?php

function vgwort_default_wsclient_service() {
  $items = array();
  $items['vgwort_service_message'] = entity_import('wsclient_service', '{
    "settings" : [],
    "operations" : {
      "newMessage" : {
        "label" : "newMessage",
        "result" : { "type" : "newMessageResponse", "label" : "newMessageResponse" },
        "parameter" : { "newMessageRequest" : { "type" : "newMessageRequest" } }
      },
      "qualityControl" : {
        "label" : "qualityControl",
        "result" : { "type" : "qualityControlResponse", "label" : "qualityControlResponse" },
        "parameter" : { "qualityControlRequest" : { "type" : "qualityControlRequest" } }
      },
      "checkAuthor" : {
        "label" : "checkAuthor",
        "result" : { "type" : "checkAuthorResponse", "label" : "checkAuthorResponse" },
        "parameter" : { "checkAuthorRequest" : { "type" : "checkAuthorRequest" } }
      },
      "addWebrange" : {
        "label" : "addWebrange",
        "result" : { "type" : "addWebrangeResponse", "label" : "addWebrangeResponse" },
        "parameter" : { "addWebrangeRequest" : { "type" : "addWebrangeRequest" } }
      }
    },
    "datatypes" : {
      "newMessageRequest" : {
        "label" : "newMessageRequest",
        "property info" : {
          "parties" : { "type" : "Parties" },
          "messagetext" : { "type" : "MessageText" },
          "webranges" : { "type" : "Webranges" },
          "privateidentificationid" : { "type" : "pixelIDType" }
        }
      },
      "newMessageResponse" : { "label" : "newMessageResponse", "property info" : [] },
      "newMessageFault" : { "label" : "newMessageFault", "property info" : [] },
      "addWebrangeRequest" : {
        "label" : "addWebrangeRequest",
        "property info" : {
          "webrange" : { "type" : "Webrange" },
          "privateidentificationid" : { "type" : "pixelIDType" }
        }
      },
      "addWebrangeResponse" : { "label" : "addWebrangeResponse", "property info" : [] },
      "addWebrangeFault" : { "label" : "addWebrangeFault", "property info" : [] },
      "qualityControlRequest" : { "label" : "qualityControlRequest", "property info" : [] },
      "qualityControlResponse" : {
        "label" : "qualityControlResponse",
        "property info" : {
          "qualityControlValues" : { "type" : "QualityControlValues" },
          "orderedPixelTillToday" : { "type" : "int_0-MAX" },
          "startedPixelTillToday" : { "type" : "int_0-MAX" }
        }
      },
      "qualityControlFault" : { "label" : "qualityControlFault", "property info" : [] },
      "checkAuthorRequest" : {
        "label" : "checkAuthorRequest",
        "property info" : {
          "cardNumber" : { "type" : "cardNumberType" },
          "surName" : { "type" : "string_2-255" }
        }
      },
      "checkAuthorResponse" : {
        "label" : "checkAuthorResponse",
        "property info" : { "valid" : { "type" : "boolean" } }
      },
      "checkAuthorFault" : { "label" : "checkAuthorFault", "property info" : [] },
      "MessageText" : {
        "label" : "MessageText",
        "property info" : {
          "shorttext" : { "type" : "string_1-100" },
          "text" : { "type" : "Text" },
          "lyric" : { "type" : "boolean" }
        }
      },
      "QualityControlValues" : {
        "label" : "QualityControlValues",
        "property info" : {
          "month" : { "type" : "int_1-12" },
          "year" : { "type" : "int_1000-9999" },
          "orderedPixel" : { "type" : "int_0-MAX" },
          "startedPixel" : { "type" : "int_0-MAX" },
          "minAccess" : { "type" : "int_0-MAX" },
          "minAccessNoMessage" : { "type" : "int_0-MAX" }
        }
      },
      "Text" : {
        "label" : "Text",
        "property info" : {
          "pdf" : { "type" : "base64BinaryMax15MB" },
          "plainText" : { "type" : "base64BinaryMax15MB" }
        }
      },
      "Parties" : {
        "label" : "Parties",
        "property info" : {
          "authors" : { "type" : "Authors" },
          "translators" : { "type" : "Translators" }
        }
      },
      "Authors" : {
        "label" : "Authors",
        "property info" : { "author" : { "type" : "Involved" } }
      },
      "Translators" : {
        "label" : "Translators",
        "property info" : { "translator" : { "type" : "Involved" } }
      },
      "Involved" : {
        "label" : "Involved",
        "property info" : {
          "code" : { "type" : "codeType" },
          "firstName" : { "type" : "string_2-40" },
          "surName" : { "type" : "string_2-255" },
          "cardNumber" : { "type" : "cardNumberType" }
        }
      },
      "Webranges" : {
        "label" : "Webranges",
        "property info" : { "webrange" : { "type" : "Webrange" } }
      },
      "Webrange" : {
        "label" : "Webrange",
        "property info" : { "url" : { "type" : "urlType" } }
      },
      "tomFault" : {
        "label" : "tomFault",
        "property info" : { "errorcode" : { "type" : "integer" }, "errormsg" : { "type" : "text" } }
      },
      "okMessage" : {
        "label" : "okMessage",
        "property info" : { "status" : { "type" : "okType" } }
      }
    },
    "global_parameters" : [],
    "name" : "vgwort_message_service",
    "label" : "VGWort Message Service",
    "url" : "https:\\/\\/tom.vgwort.de\\/services\\/1.2\\/messageService.wsdl",
    "type" : "exsoap",
    "authentication" : null,
    "rdf_mapping" : []
  }');
  $items['vgwort_service_pixel'] = entity_import('wsclient_service', '{
    "settings" : [],
    "operations" : {
      "orderPixel" : {
        "label" : "orderPixel",
        "result" : { "type" : "orderPixelResponse", "label" : "orderPixelResponse" },
        "parameter" : { "orderPixelRequest" : { "type" : "orderPixelRequest" } }
      },
      "pixelOverview" : {
        "label" : "pixelOverview",
        "result" : { "type" : "pixelOverviewResponse", "label" : "pixelOverviewResponse" },
        "parameter" : { "pixelOverviewRequest" : { "type" : "pixelOverviewRequest" } }
      }
    },
    "datatypes" : {
      "orderPixelRequest" : {
        "label" : "orderPixelRequest",
        "property info" : { "count" : { "type" : "int_1-Max" } }
      },
      "orderPixelResponse" : {
        "label" : "orderPixelResponse",
        "property info" : {
          "domain" : { "type" : "text" },
          "orderDateTime" : { "type" : "stringDateType" },
          "pixels" : { "type" : "Pixels" }
        }
      },
      "orderPixelFault" : {
        "label" : "orderPixelFault",
        "property info" : { "maxOrder" : { "type" : "int_0-Max" } }
      },
      "pixelOverviewRequest" : {
        "label" : "pixelOverviewRequest",
        "property info" : {
          "offset" : { "type" : "int_0-Max" },
          "type" : { "type" : "pixelOverviewType" }
        }
      },
      "pixelOverviewResponse" : {
        "label" : "pixelOverviewResponse",
        "property info" : {
          "privateIdentificationId" : { "type" : "pixelIDType" },
          "offset" : { "type" : "int_0-Max" },
          "amount" : { "type" : "int_0-Max" }
        }
      },
      "pixelOverviewFault" : { "label" : "pixelOverviewFault", "property info" : [] },
      "Pixels" : {
        "label" : "Pixels",
        "property info" : { "pixel" : { "type" : "Pixel" } }
      },
      "Pixel" : {
        "label" : "Pixel",
        "property info" : {
          "publicIdentificationId" : { "type" : "pixelIDType" },
          "privateIdentificationId" : { "type" : "pixelIDType" }
        }
      },
      "tomFault" : {
        "label" : "tomFault",
        "property info" : { "errorcode" : { "type" : "integer" }, "errormsg" : { "type" : "text" } }
      }
    },
    "global_parameters" : [],
    "name" : "vgwort_pixel_service",
    "label" : "VGWort Pixel Service",
    "url" : "https:\\/\\/tom.vgwort.de\\/services\\/1.0\\/pixelService.wsdl",
    "type" : "exsoap",
    "authentication" : null,
    "rdf_mapping" : []
  }');

  $items['vgwort_service_message_test'] = entity_import('wsclient_service', '{
    "settings" : [],
    "operations" : {
      "newMessage" : {
        "label" : "newMessage",
        "result" : { "type" : "newMessageResponse", "label" : "newMessageResponse" },
        "parameter" : { "newMessageRequest" : { "type" : "newMessageRequest" } }
      },
      "qualityControl" : {
        "label" : "qualityControl",
        "result" : { "type" : "qualityControlResponse", "label" : "qualityControlResponse" },
        "parameter" : { "qualityControlRequest" : { "type" : "qualityControlRequest" } }
      },
      "checkAuthor" : {
        "label" : "checkAuthor",
        "result" : { "type" : "checkAuthorResponse", "label" : "checkAuthorResponse" },
        "parameter" : { "checkAuthorRequest" : { "type" : "checkAuthorRequest" } }
      },
      "addWebrange" : {
        "label" : "addWebrange",
        "result" : { "type" : "addWebrangeResponse", "label" : "addWebrangeResponse" },
        "parameter" : { "addWebrangeRequest" : { "type" : "addWebrangeRequest" } }
      }
    },
    "datatypes" : {
      "newMessageRequest" : {
        "label" : "newMessageRequest",
        "property info" : {
          "parties" : { "type" : "Parties" },
          "messagetext" : { "type" : "MessageText" },
          "webranges" : { "type" : "Webranges" },
          "privateidentificationid" : { "type" : "pixelIDType" }
        }
      },
      "newMessageResponse" : { "label" : "newMessageResponse", "property info" : [] },
      "newMessageFault" : { "label" : "newMessageFault", "property info" : [] },
      "addWebrangeRequest" : {
        "label" : "addWebrangeRequest",
        "property info" : {
          "webrange" : { "type" : "Webrange" },
          "privateidentificationid" : { "type" : "pixelIDType" }
        }
      },
      "addWebrangeResponse" : { "label" : "addWebrangeResponse", "property info" : [] },
      "addWebrangeFault" : { "label" : "addWebrangeFault", "property info" : [] },
      "qualityControlRequest" : { "label" : "qualityControlRequest", "property info" : [] },
      "qualityControlResponse" : {
        "label" : "qualityControlResponse",
        "property info" : {
          "qualityControlValues" : { "type" : "QualityControlValues" },
          "orderedPixelTillToday" : { "type" : "int_0-MAX" },
          "startedPixelTillToday" : { "type" : "int_0-MAX" }
        }
      },
      "qualityControlFault" : { "label" : "qualityControlFault", "property info" : [] },
      "checkAuthorRequest" : {
        "label" : "checkAuthorRequest",
        "property info" : {
          "cardNumber" : { "type" : "cardNumberType" },
          "surName" : { "type" : "string_2-255" }
        }
      },
      "checkAuthorResponse" : {
        "label" : "checkAuthorResponse",
        "property info" : { "valid" : { "type" : "boolean" } }
      },
      "checkAuthorFault" : { "label" : "checkAuthorFault", "property info" : [] },
      "MessageText" : {
        "label" : "MessageText",
        "property info" : {
          "shorttext" : { "type" : "string_1-100" },
          "text" : { "type" : "Text" },
          "lyric" : { "type" : "boolean" }
        }
      },
      "QualityControlValues" : {
        "label" : "QualityControlValues",
        "property info" : {
          "month" : { "type" : "int_1-12" },
          "year" : { "type" : "int_1000-9999" },
          "orderedPixel" : { "type" : "int_0-MAX" },
          "startedPixel" : { "type" : "int_0-MAX" },
          "minAccess" : { "type" : "int_0-MAX" },
          "minAccessNoMessage" : { "type" : "int_0-MAX" }
        }
      },
      "Text" : {
        "label" : "Text",
        "property info" : {
          "pdf" : { "type" : "base64BinaryMax15MB" },
          "plainText" : { "type" : "base64BinaryMax15MB" }
        }
      },
      "Parties" : {
        "label" : "Parties",
        "property info" : {
          "authors" : { "type" : "Authors" },
          "translators" : { "type" : "Translators" }
        }
      },
      "Authors" : {
        "label" : "Authors",
        "property info" : { "author" : { "type" : "Involved" } }
      },
      "Translators" : {
        "label" : "Translators",
        "property info" : { "translator" : { "type" : "Involved" } }
      },
      "Involved" : {
        "label" : "Involved",
        "property info" : {
          "code" : { "type" : "codeType" },
          "firstName" : { "type" : "string_2-40" },
          "surName" : { "type" : "string_2-255" },
          "cardNumber" : { "type" : "cardNumberType" }
        }
      },
      "Webranges" : {
        "label" : "Webranges",
        "property info" : { "webrange" : { "type" : "Webrange" } }
      },
      "Webrange" : {
        "label" : "Webrange",
        "property info" : { "url" : { "type" : "urlType" } }
      },
      "tomFault" : {
        "label" : "tomFault",
        "property info" : { "errorcode" : { "type" : "integer" }, "errormsg" : { "type" : "text" } }
      },
      "okMessage" : {
        "label" : "okMessage",
        "property info" : { "status" : { "type" : "okType" } }
      }
    },
    "global_parameters" : [],
    "name" : "vgwort_message_service",
    "label" : "VGWort Message Service (Test)",
    "url" : "https:\\/\\/tom-test.vgwort.de\\/services\\/1.2\\/messageService.wsdl",
    "type" : "exsoap",
    "authentication" : null,
    "rdf_mapping" : []
  }');
  $items['vgwort_service_pixel_test'] = entity_import('wsclient_service', '{
    "settings" : [],
    "operations" : {
      "orderPixel" : {
        "label" : "orderPixel",
        "result" : { "type" : "orderPixelResponse", "label" : "orderPixelResponse" },
        "parameter" : { "orderPixelRequest" : { "type" : "orderPixelRequest" } }
      },
      "pixelOverview" : {
        "label" : "pixelOverview",
        "result" : { "type" : "pixelOverviewResponse", "label" : "pixelOverviewResponse" },
        "parameter" : { "pixelOverviewRequest" : { "type" : "pixelOverviewRequest" } }
      }
    },
    "datatypes" : {
      "orderPixelRequest" : {
        "label" : "orderPixelRequest",
        "property info" : { "count" : { "type" : "int_1-Max" } }
      },
      "orderPixelResponse" : {
        "label" : "orderPixelResponse",
        "property info" : {
          "domain" : { "type" : "text" },
          "orderDateTime" : { "type" : "stringDateType" },
          "pixels" : { "type" : "Pixels" }
        }
      },
      "orderPixelFault" : {
        "label" : "orderPixelFault",
        "property info" : { "maxOrder" : { "type" : "int_0-Max" } }
      },
      "pixelOverviewRequest" : {
        "label" : "pixelOverviewRequest",
        "property info" : {
          "offset" : { "type" : "int_0-Max" },
          "type" : { "type" : "pixelOverviewType" }
        }
      },
      "pixelOverviewResponse" : {
        "label" : "pixelOverviewResponse",
        "property info" : {
          "privateIdentificationId" : { "type" : "pixelIDType" },
          "offset" : { "type" : "int_0-Max" },
          "amount" : { "type" : "int_0-Max" }
        }
      },
      "pixelOverviewFault" : { "label" : "pixelOverviewFault", "property info" : [] },
      "Pixels" : {
        "label" : "Pixels",
        "property info" : { "pixel" : { "type" : "Pixel" } }
      },
      "Pixel" : {
        "label" : "Pixel",
        "property info" : {
          "publicIdentificationId" : { "type" : "pixelIDType" },
          "privateIdentificationId" : { "type" : "pixelIDType" }
        }
      },
      "tomFault" : {
        "label" : "tomFault",
        "property info" : { "errorcode" : { "type" : "integer" }, "errormsg" : { "type" : "text" } }
      }
    },
    "global_parameters" : [],
    "name" : "vgwort_pixel_service",
    "label" : "VGWort Pixel Service (Test)",
    "url" : "https:\\/\\/tom-test.vgwort.de\\/services\\/1.0\\/pixelService.wsdl",
    "type" : "exsoap",
    "authentication" : null,
    "rdf_mapping" : []
  }');

  return $items;
}
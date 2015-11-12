(($) ->
  Drupal.behaviors.MoipCT = attach: (context, settings) ->
    
    # Disable "continue checkout" button when Moip CT is checked or if it
    # starts the page already checked
    $("#edit-commerce-payment-payment-method.form-radios input", context).change ->
      if $(this).val() is "moip_ct|commerce_payment_moip_ct"
        $("#edit-buttons input.checkout-continue").hide()
        $("#edit-buttons").addClass "moip-ct"
      else
        $("#moip-ct-js-form").hide()
        $("#edit-buttons input.checkout-continue").show()
        $("#edit-buttons").removeClass "moip-ct"
      return
    
    # Validate credit card number on blur
    $(".payment-way.creditcard .number input", context).blur ->
      if moip.creditCard.isValid $(this).val()
        card = moip.creditCard.cardType $(this).val()
        $(".payment-way.creditcard .number .help")
          .empty()
          .removeClass()
          .addClass "help"
          .addClass "payment-icon-creditcard-" + card.brand.toLowerCase()
          .css "width", "60px"
          .css "height", "35px"
          .css "margin", "0 0 -15px"
      else
        $(".payment-way.creditcard .number .help")
          .removeClass()
          .addClass "help"
          .removeAttr "style"
      return
    
    # Limit credit card security code lenght
    $(".payment-way.creditcard .securitycode input", context).keyup ->
      securitycode = $(this).val()
      if securitycode.length > 4
        $(this).val securitycode.substring(0, securitycode.length - 1)  
      return

    # Manages the exibition of payment way forms
    $("#edit-commerce-payment-payment-details-moip-ct-payment-way .form-radio", context).change ->
      
      $("#moip-ct-messages").empty().hide()
      $("#moip-ct-buttons").show()
      $("#moip-ct-js-form .payment-way").hide()
      $("#moip-ct-js-form .payment-way." + @value).show()
      
      if @value is "creditcard"
        $("#moip-ct-submit").text Drupal.t("Pay with credit card")
      else if @value is "bankbillet"
        $("#moip-ct-submit").text Drupal.t("Pay with bank billet")
      else if @value is "banktransfer"
        $("#moip-ct-submit").text Drupal.t("Pay with bank transfer")
      
      moip_ct_button = $("#moip-ct-buttons").clone()
      $("#moip-ct-buttons").remove()
      $("#edit-buttons .fieldset-wrapper").prepend moip_ct_button
      return
    return

  # Disable the original submit button so that we can handle the data
  # and send it to MoIP, submiting the form after via JS
  $("#edit-commerce-payment-payment-details-moip-ct").ready ->
    $("#edit-buttons input.checkout-continue").hide()
    $("#edit-buttons").addClass "moip-ct"
    return

  Drupal.Moip = {}
  Drupal.Moip.CT = {}
  Drupal.Moip.CT.send = (order_id) ->
    try
      
      # Disable other payment methods to avoid conflict
      $("#edit-commerce-payment-payment-method.form-radios input:not(:checked)").attr "disabled", "disabled"
      Drupal.Moip.CT.paymentway = $("#edit-commerce-payment-payment-details-moip-ct-payment-way .form-radio:checked").val()
      
      # Disable other payment options to avoid multiple submits to MoIP
      $("#edit-commerce-payment-payment-details-moip-ct-payment-way .form-radio:not(:checked)").parents(".form-item.form-type-radio").hide()
      
      # Hide js form buttons
      $("#moip-ct-buttons").hide()
      
      switch Drupal.Moip.CT.paymentway
        when "creditcard"
          Drupal.Moip.CT.sendCreditCard()
        when "bankbillet"
          Drupal.Moip.CT.sendBankBillet()
        when "banktransfer"
          Drupal.Moip.CT.sendBankTransfer()
        
    catch error
      txt = "Sorry, we have an undefined error in the system. <br/>"
      txt += "Our team was notified and we will fix it asap."
      if typeof jdog_error is "function"
        jdog_error "moip_js_error", Drupal.t(error.message)  
      Drupal.Moip.CT.UI.errorAlert Drupal.t(txt)
    return

  Drupal.Moip.CT.sendCreditCard = ->
    
    # Hide unnecessary elements and show the throbber
    $("#moip-ct-js-form > :not(.ajax-progress-throbber)").hide()
    $("#moip-ct-js-form .ajax-progress-throbber").show()
    
    # We don't need to validate only the Phone here, as it was done on server
    # side, but need to every other fields
    phone = $("#moip-user-phone").val()
  
    name = $(".payment-way.creditcard .name input").val()
    unless name?.length
      Drupal.Moip.CT.UI.sendCreditCardErrorAlert Drupal.t('The "name" field is required.')
      return false
    
    birthday = $(".payment-way.creditcard .birthday input").val()
    unless birthday?.length
      Drupal.Moip.CT.UI.sendCreditCardErrorAlert Drupal.t('The "birthday" field is required.')
      return false
      
    cpf = $(".payment-way.creditcard .cpf input").val()
    unless cpf?.length
      Drupal.Moip.CT.UI.sendCreditCardErrorAlert Drupal.t('The "cpf" field is required.')
      return false
    
    number = $(".payment-way.creditcard .number input").val()
    unless number?.length
      Drupal.Moip.CT.UI.sendCreditCardErrorAlert Drupal.t('The "number" field is required.')
      return false
    unless moip.creditCard.isValid(number)
      Drupal.Moip.CT.UI.sendCreditCardErrorAlert Drupal.t("The informed card number is invalid. Please verify.")
      return false

    security_code = $(".payment-way.creditcard .securitycode input").val()
    unless security_code?.length
      Drupal.Moip.CT.UI.sendCreditCardErrorAlert Drupal.t('The "security code" field is required.')
      return false
    unless moip.creditCard.isSecurityCodeValid(number, security_code)
      Drupal.Moip.CT.UI.sendCreditCardErrorAlert Drupal.t("The informed security code is invalid. Please verify.")
      return false

    expiration_date = $(".payment-way.creditcard .expirationdate input").val()
    unless expiration_date?.length
      Drupal.Moip.CT.UI.sendCreditCardErrorAlert Drupal.t('The "expiration date" field is required.')
      return false
    unless expiration_date.length == 7 and expiration_date.charAt(2) == '/'
      Drupal.Moip.CT.UI.sendCreditCardErrorAlert Drupal.t('The "expiration date" should be informed in the format "mm/yyyy".')
      return false
    expiration_date_split = expiration_date.split("/")
    unless moip.creditCard.isExpiryDateValid(expiration_date_split[0], expiration_date_split[1])
      Drupal.Moip.CT.UI.sendCreditCardErrorAlert Drupal.t('The "expiration date" informed is invalid.')
      return false
    
    card = moip.creditCard.cardType(number)
    paymentwayoption = undefined
    
    # Save card brand on the form to be processed by Form API
    switch card.brand
      when "AMEX"
        paymentwayoption = "AmericanExpress"
      when "DINERS"
        paymentwayoption = "Diners"
      when "HIPERCARD"
        paymentwayoption = "Hipercard"
      when "MASTERCARD"
        paymentwayoption = "Mastercard"
      when "VISA"
        paymentwayoption = "Visa"
      
    $(".moip-ct-paymentwayoption").val paymentwayoption
    
    settings =
      Forma: "CartaoCredito"
      Instituicao: paymentwayoption
      Parcelas: "1"
      Recebimento: "AVista"
      CartaoCredito:
        Numero: number
        Expiracao: expiration_date
        CodigoSeguranca: security_code
        Portador:
          Nome: name
          DataNascimento: birthday
          Telefone: phone
          Identidade: cpf

    MoipWidget settings
    return

  Drupal.Moip.CT.sendBankBillet = ->
    settings = Forma: "BoletoBancario"
    MoipWidget settings
    return

  Drupal.Moip.CT.sendBankTransfer = ->
    option = $(".payment-way.banktransfer input:checked").val()
    unless option?
      Drupal.Moip.CT.UI.errorAlert Drupal.t("You need to specify the bank.")
      $("#edit-commerce-payment-payment-details-moip-ct-payment-way .form-type-radio").show()
    else
      # Hide other options that weren't selected
      $(".payment-way.banktransfer input:not(:checked)").parents(".field-wrapper").hide()
      $(".moip-ct-paymentwayoption").val option
      settings =
        Forma: "DebitoBancario"
        Instituicao: option
      MoipWidget settings
    return

  Drupal.Moip.CT.sendSuccesfull = (data) ->

    Drupal.Moip.CT.answer = JSON.stringify(data)

    if data.Status is "Cancelado"
      Drupal.Moip.CT.UI.sendCreditCardErrorAlert eval("Drupal.Moip.Error.CreditCardErrorMap.e" + data.Classificacao.Codigo)
    else
      switch Drupal.Moip.CT.paymentway
        when "creditcard"
          Drupal.Moip.CT.submitCheckoutForm()
        when "bankbillet"
          Drupal.Moip.CT.UI.externalPaymentUrlAlert "bankbillet", data.url
        when "banktransfer"
          Drupal.Moip.CT.UI.externalPaymentUrlAlert "banktransfer", data.url
    return

  Drupal.Moip.CT.submitCheckoutForm = ->
    if Drupal.Moip.CT.answer.length is 0
      if typeof jdog_error is "function"
        jdog_error "moip_js_error", Drupal.t('The Moip "Answer" came empty')
    else
      $(".moip-ct-answer").val Drupal.Moip.CT.answer
      
      # The following line is fundamental to PCI compliance, avoiding
      # the data to be sent to server
      $("#moip-ct-js-form").remove()
      $(".moip-ct-answer").parents("form").submit()
    return

  Drupal.Moip.CT.sendFailed = (data) ->
    
    # See more in https://labs.moip.com.br/parametro/erros-js
    error_map =
      c900: "Forma de pagamento inválida"
      c901: "Informe a instituição de pagamento"
      c902: "Informe a quantidade de parcelas (valor entre 1 e 12)"
      c903: "Tipo de recebimento inválido"
      c904: "Número de cartão ou Cofre deve ser informado"
      c905: "Informe o número do cartão ou número do cartão é inválido"
      c906: "Informe a data de expiração do cartão (no formato MM/AA)"
      c907: "Informe o código de segurança do cartão ou o código é inválido"
      c908: "Informe os dados do portador do cartão"
      c909: "Informe o nome do portador como está no cartão"
      c910: "Informe a data de nascimento do portador (no formato DD/MM/AAAA)"
      c911: "Informe o telefone do portador ou o telefone é inválido"
      c912: "Informe o CPF do portador"
      c913: "Informe o cofre a ser utilizado"
      c914: "Informe o token da Instrução"
      c124: "O valor da parcela deve ser superior a R$5,00"
      c236: "Este pagamento já foi realizado"

    errorArr = $.makeArray(data)
    messages = ""
    $.each errorArr, (index, value) ->
      msg = eval("error_map.c" + value.Codigo)
      if msg
        messages += msg + "<br />"
      return

    if messages.length is 0 # deals with other messages not mapped
      $.each errorArr, (index, value) ->
        messages += value.Mensagem + "<br />"
        return

    Drupal.Moip.CT.UI.sendCreditCardErrorAlert messages
    return

  Drupal.Moip.CT.UI = {}
  Drupal.Moip.CT.UI.messageAlert = (message) ->
    $("#moip-ct-messages").empty()
    $("#moip-ct-messages").removeClass()
    $("#moip-ct-messages").append message
    $("#moip-ct-messages").show effect: "highlight"
    return

  Drupal.Moip.CT.UI.errorAlert = (message) ->
    Drupal.Moip.CT.UI.messageAlert message
    $("#moip-ct-messages").addClass "error"
    $("#moip-ct-buttons").show()
    return

  Drupal.Moip.CT.UI.infoAlert = (message) ->
    Drupal.Moip.CT.UI.messageAlert message
    $("#moip-ct-messages").addClass "info"
    return

  Drupal.Moip.CT.UI.sendCreditCardErrorAlert = (message) ->
    Drupal.Moip.CT.UI.errorAlert message
    $("#moip-ct-buttons").show()
    $(".form-item-commerce-payment-payment-details-moip-ct-payment-way").show()
    $("#moip-ct-js-form .payment-way.creditcard").show()
    $("#moip-ct-js-form .ajax-progress-throbber").hide()
    return

  Drupal.Moip.CT.UI.externalPaymentUrlAlert = (paymentway, url) ->
    args = "!link": url
    switch paymentway
      when "bankbillet"
        message = Drupal.t('<p>To complete your payment, <a data-href="!link">click here</a> to open your billet in another browser window.</p><p>You will be redirected automatically after this.</p>', args)
      when "banktransfer"
        message = Drupal.t('<p>To complete your payment, <a data-href="!link">click here</a> to open your internet banking in another browser window.</p><p>You will be redirected automatically after this.</p>', args)
    Drupal.Moip.CT.UI.infoAlert message
    
    $("#moip-ct-messages a").click ->
      # The link needs to be opened this way to avoid problems with some browsers that submit
      # the form without opening the url in another window/tab
      window.open $(this).data("href")
      Drupal.Moip.CT.submitCheckoutForm()
      return
    return

  Drupal.Moip.Error = {}
  Drupal.Moip.Error.CreditCardErrorMap =
    
    # See the errors on https://labs.moip.com.br/referencia/classificacao-de-cancelamento/
    e1: "Os dados informados foram considerados inválidos pelo banco. Verifique se digitou algo errado e tente novamente."
    e2: "Houve uma falha de comunicação com a operadora/banco do seu cartão. Você pode tentar novamente ou tentar em outro momento."
    e3: "O pagamento não foi autorizado pela operadora/banco do seu cartão. Favor entrar em contato com ela para esclarecer o problema e tentar novamente em seguida."
    e4: "A validade do seu cartão expirou. Caso deseje, você pode escolher outra forma de pagamento."
    e5: "O pagamento não foi autorizado pela operadora/banco do seu cartão. Caso deseje, você pode escolher outra forma de pagamento."
    e6: "Esse pagamento já foi realizado. Favor entrar em contato conosco para verificarmos junto ao sistema."
    e7: "O pagamento não foi autorizado. Para mais informações, entre em contato com nosso atendimento."
    e8: "O pagamento não pode ser processado. Favor tentar novamente. Caso o erro persista, entre em contato com nosso atendimento."
    e11: "Houve uma falha de comunicação com a operador/banco do seu cartão. Favor tentar novamente."
    e12: "O pagamento não foi autorizado para este cartão. Favor entrar em contato com a operadora/banco para mais esclarecimentos."
    e13: "O pagamento não foi autorizado. Favor entrar em contato com nosso atendimento."
    e14: "O pagamento não foi autorizado. Favor entrar em contato com a operador/banco do seu cartão para maiores detalhes."

  return
) jQuery

<?php

/**
 * @file
 * Contains the classes used in commerce_nets.
 * @todo Documentation??
 */

class Terminal {

  public $AutoAuth;
  public $PaymentMethodList;
  public $Language;
  public $OrderDescription;
  public $RedirectOnError;
  public $RedirectUrl;

  function Terminal
  (
  $AutoAuth, $PaymentMethodList, $Language, $OrderDescription, $RedirectOnError, $RedirectUrl
  ) {
    $this->AutoAuth = $AutoAuth;
    $this->PaymentMethodList = $PaymentMethodList;
    $this->Language = $Language;
    $this->OrderDescription = $OrderDescription;
    $this->RedirectOnError = $RedirectOnError;
    $this->RedirectUrl = $RedirectUrl;
  }

}

class RegisterRequest {

  public $AvtaleGiro;
  public $CardInfo;
  public $Customer;
  public $Description;
  public $DnBNorDirectPayment;
  public $Environment;
  public $MicroPayment;
  public $Order;
  public $Recurring;
  public $ServiceType;
  public $Terminal;
  public $TransactionId;
  public $TransactionReconRef;

  function RegisterRequest
  (
  $AvtaleGiro, $CardInfo, $Customer, $Description, $DnBNorDirectPayment, $Environment, $MicroPayment, $Order, $Recurring, $ServiceType, $Terminal, $TransactionId, $TransactionReconRef
  ) {
    $this->AvtaleGiro = $AvtaleGiro;
    $this->CardInfo = $CardInfo;
    $this->Customer = $Customer;
    $this->Description = $Description;
    $this->DnBNorDirectPayment = $DnBNorDirectPayment;
    $this->Environment = $Environment;
    $this->MicroPayment = $MicroPayment;
    $this->Order = $Order;
    $this->Recurring = $Recurring;
    $this->ServiceType = $ServiceType;
    $this->Terminal = $Terminal;
    $this->TransactionId = $TransactionId;
    $this->TransactionReconRef = $TransactionReconRef;
  }

}

class ProcessRequest {

  public $Description;
  public $Operation;
  public $TransactionAmount;
  public $TransactionId;
  public $TransactionReconRef;

  function ProcessRequest
  (
  $Description, $Operation, $TransactionAmount, $TransactionId, $TransactionReconRef
  ) {
    $this->Description = $Description;
    $this->Operation = $Operation;
    $this->TransactionAmount = $TransactionAmount;
    $this->TransactionId = $TransactionId;
    $this->TransactionReconRef = $TransactionReconRef;
  }

}

class Order {

  public $Amount;
  public $CurrencyCode;
  public $Force3DSecure;
  public $Goods;
  public $OrderNumber;
  public $UpdateStoredPaymentInfo;

  function Order
  (
  $Amount, $CurrencyCode, $Force3DSecure, $Goods, $OrderNumber, $UpdateStoredPaymentInfo
  ) {
    $this->Amount = $Amount;
    $this->CurrencyCode = $CurrencyCode;
    $this->Force3DSecure = $Force3DSecure;
    $this->Goods = $Goods;
    $this->OrderNumber = $OrderNumber;
    $this->UpdateStoredPaymentInfo = $UpdateStoredPaymentInfo;
  }

}

class Item {

  public $Amount;
  public $ArticleNumber;
  public $Discount;
  public $Handling;
  public $IsVatIncluded;
  public $Quantity;
  public $Shipping;
  public $Title;
  public $VAT;

  function Item
  (
  $Amount, $ArticleNumber, $Discount, $Handling, $IsVatIncluded, $Quantity, $Shipping, $Title, $VAT
  ) {
    $this->Amount = $Amount;
    $this->ArticleNumber = $ArticleNumber;
    $this->Discount = $Discount;
    $this->Handling = $Handling;
    $this->IsVatIncluded = $IsVatIncluded;
    $this->Quantity = $Quantity;
    $this->Shipping = $Shipping;
    $this->Title = $Title;
    $this->VAT = $VAT;
  }

}

class Environment {

  public $Language;
  public $OS;
  public $WebServicePlatform;

  function Environment
  (
  $Language, $OS, $WebServicePlatform
  ) {
    $this->Language = $Language;
    $this->OS = $OS;
    $this->WebServicePlatform = $WebServicePlatform;
  }

}

class Customer {

  public $Address1;
  public $Address2;
  public $CompanyName;
  public $CompanyRegistrationNumber;
  public $Country;
  public $CustomerNumber;
  public $Email;
  public $FirstName;
  public $LastName;
  public $PhoneNumber;
  public $Postcode;
  public $SocialSecurityNumber;
  public $Town;

  function Customer
  (
  $Address1, $Address2, $CompanyName, $CompanyRegistrationNumber, $Country, $CustomerNumber, $Email, $FirstName, $LastName, $PhoneNumber, $Postcode, $SocialSecurityNumber, $Town
  ) {
    $this->Address1 = $Address1;
    $this->Address2 = $Address2;
    $this->CompanyName = $CompanyName;
    $this->CompanyRegistrationNumber = $CompanyRegistrationNumber;
    $this->Country = $Country;
    $this->CustomerNumber = $CustomerNumber;
    $this->Email = $Email;
    $this->FirstName = $FirstName;
    $this->LastName = $LastName;
    $this->PhoneNumber = $PhoneNumber;
    $this->Postcode = $Postcode;
    $this->SocialSecurityNumber = $SocialSecurityNumber;
    $this->Town = $Town;
  }

}

class ArrayOfItem {

  public $Item;

  function ArrayOfItem
  (
  $Item
  ) {
    $this->Item = $Item;
  }

}

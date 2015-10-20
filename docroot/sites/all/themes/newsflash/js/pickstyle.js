function pickstyle(whichstyle) {
  var expireDate = new Date()
  expireDate.setDate(expireDate.getDate() + 30)
//  expiresDate.setTime(expiresDate.getTime()+(1000*3600*24*730)); // 730 Tage = 2 Jahre

  document.cookie = "newsflashstyle=" + whichstyle + "; expires=" + expireDate.toGMTString() + ";";
}

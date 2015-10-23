Eklenti ve banka sanalPOS mağaza yöntemleri ile ilgili ön hazırlık:
------------------------------------------------------------------
Eklentiyi kullanabilmek için aşağıda listesini görebileceğini bankalardan biri ile sanal POS anlaşmasına ihtiyaç bulunmaktadır. 
Bankanızdan alacağınız mağaza bilgilerini eklentiyi kurduktan sonra tanımlayabilir ve mağazanızı satışa hazırlayabilirsiniz.

Eklentinin desteklediği bankalar:
---------------------------------
-İş Bankası
-Akbank
-Anadolubank
-KuveytTürk
-Finansbank
-Halkbank
-Denizbank
-Citibank
-TEB
-Cardplus
-İngBank

Eklenti iki tür sanalPOS mağaza tipini desteklemektedir. Bunları: 3D ve 3D Hosting yöntemleridir.

3D yöntemi
----------
Bu yöntemde kart bilgileri sitenizde müşteriden alınır ve sonrasında müşteri kart doğrulama için banka web sitesine gönderilir. 
Bankada 3D doğrulaması yapıldıktan sonra tekrar web sitesine müşteri bir anahtar ile döner ve bu anahtar ile banka API sunucularından otorizasyon alınarak süreç sonlandırılır.
Bu yöntemin çalıştırılabilmesi için web sitenizin SSL sertifikası olması gerekmektedir. 
Ayrıca ubercart'ın da kredi kartı şifreleme yapabilmesi için basit bir işlem de yapılması gerekmektedir.

3D Hosting yöntemi
------------------
Bu yöntemde web siteniz müşterinin kart bilgisini almaz. 
Özet bilgisini inceleyen müşteri kart bilgilerini girmek ve otorizasyon işlemini tamamlamak üzere bankanızın web sitesine gönderilir. 
Şifre kontrolünden geçen ve ödemesi tamamlanan müşteri tekrar siteye döner ve işlem sonlandırılır.
Bu yöntemde SSL ve şifrele ile ilgili herhangi bir ihtiyaç bulunmaz.

Not: Eğer isterseniz iki metodu da aynı anda müşterilerilerinize sunabilirsiniz. Bunun için bankanızdan iki yöntem için gerekli parametreleri almanız yeterlidir.

Kurulum için: (drupal ve ubercart kurulmuştur sanırım)
------------------------------------------------------

-Eklentiyi indirin.
-/sites/all/modules/uc_turkish_banks şeklinde açın.
-Sitenize girin ve eklentiyi etkinleştirin. Eklentinin adı Turkish Banks Gateway.
-Drupal'in menüsünden "Store" bağlantısını tıklayın.
-Gelen ekranda "Configuration" kutucuğunda "Payment Methods" bağlantısını tıklayın.
-Karşınıza iki seçenek gelecek. "Credit card (includes Anlaşmalı Bankanız (3D))" ve "Turkish Banks (3D Hosting)".
-Bu aşamadan kullanacağızın sanalPOS mağaza tipine göre aşağıdaki iki alternatif süreçten ilgili olanı yapmanız gerekiyor.

*"Credit card (includes Anlaşmalı Bankanız (3D))"
-"Payment Method" sayfasında "Settings" bağlantısını tıklayın.
-Sol menüdeki "Security settings" için bir "Encryption key directory" vermeniz gerekiyor. 3D yönteminin çalışması için bu gerekli.
-Yine sol menüde "Anlaşmalı Bankanız (3D)"ı seçin.
-"Enable this payment gateway for use." seçeneğinin tıklanmış olduğundan emin olun.
-Bankanızdan temin ettiğin mağaza tanımların burada tanımlayın. DIKKAT! TEST ETME NİYETİNDE İSENİZ BU PARAMETRELER TEST ORTAMININ OLMALI!
-Kayıt edin ve çıkın.
-3D metodu alışveriş yapabilirsiniz artık!

*"Turkish Banks (3D Hosting)"
-"Payment Method" sayfasında "Settings" bağlantısını tıklayın.
-Bankanızdan temin ettiğin mağaza tanımların burada tanımlayın. DIKKAT! TEST ETME NİYETİNDE İSENİZ BU PARAMETRELER TEST ORTAMININ OLMALI!
-Kayıt edin ve çıkın.
-3D Hosting metodu ile alışveriş yapabilirsiniz artık!
r(200,188,{});function rO(){rO=s;sO=new Zn(xd,new tO)}function uO(a){a.a.stopPropagation();a.a.preventDefault()}function tO(){}r(201,200,{},tO);_.Pc=function(){uO(this)};_.Sc=function(){return sO};var sO;function vO(){vO=s;wO=new Zn(yd,new xO)}function xO(){}r(202,200,{},xO);_.Pc=function(){uO(this)};_.Sc=function(){return wO};var wO;function yO(){yO=s;zO=new Zn(zd,new AO)}function AO(){}r(203,200,{},AO);_.Pc=function(){uO(this)};_.Sc=function(){return zO};var zO;
function BO(){BO=s;CO=new Zn(Ad,new DO)}function DO(){}r(204,200,{},DO);_.Pc=function(a){var b,c,d,e;this.a.stopPropagation();this.a.preventDefault();d=(this.a.dataTransfer||null).files;e=0;a:for(;e<d.length;++e){if(0<a.a.d&&e>=a.a.d)break a;b=d[e];c=new FileReader;EO(c,a.a.b);1==a.a.c&&c.readAsText(b)}0==d.length&&(b=(this.a.dataTransfer||null).getData(dg),a.a.b.a.a.e.ob[tg]=null!=b?b:l)};_.Sc=function(){return CO};var CO;
function FO(a,b,c){var d=a.ob,e=c.b;hr();Rr(d,e);Ap(!a.lb?a.lb=new Pp(a):a.lb,c,b)}function GO(a){var b=$doc.createElement(qd);lH(Tf,b.tagName);this.ob=b;this.b=new fI(this.ob);this.ob[Uc]="gwt-HTML";eI(this.b,a,!0);nI(this)}r(321,322,{13:1,15:1,17:1,18:1,20:1,21:1,22:1,23:1,24:1,25:1,26:1,27:1,28:1,30:1,31:1,32:1,36:1,37:1,38:1,39:1,40:1,41:1,42:1,43:1,44:1,45:1,47:1,49:1,53:1,58:1,68:1,69:1,70:1,73:1,77:1,80:1,81:1,83:1},GO);
function HO(){Nu();var a=$doc.createElement("textarea");!Zq&&(Zq=new Yq);!Xq&&(Xq=new Wq);this.ob=a;this.ob[Uc]="gwt-TextArea"}r(361,362,Mh,HO);function IO(a,b){var c,d;c=$doc.createElement(og);d=$doc.createElement(cg);d[vc]=a.a.a;d.style[ug]=a.b.a;var e=(ar(),br(d));c.appendChild(e);$q(a.d,c);xs(a,b,d)}function JO(){Et.call(this);this.a=(Ht(),Ot);this.b=(Pt(),St);this.e[Qc]=Ya;this.e[Pc]=Ya}r(370,315,Ih,JO);_.Dd=function(a){var b;b=ym(a.ob);(a=Bs(this,a))&&this.d.removeChild(ym(b));return a};
function KO(a){try{a.v=!1;var b,c,d;d=a.gb;c=a._;d||(a.ob.style[vg]=ce,a._=!1,a.Qd());b=a.ob;b.style[me]=0+(sn(),qf);b.style[jg]=Za;XJ(a,Ai(Fm($doc)+(Em()-um(a.ob,df)>>1),0),Ai(Gm($doc)+(Dm()-um(a.ob,cf)>>1),0));d||((a._=c)?(a.ob.style[dd]=wf,a.ob.style[vg]=zg,bi(a.fb,200)):a.ob.style[vg]=zg)}finally{a.v=!0}}function LO(a){a.f=(new iJ(a.i)).mc.Fe();hs(a.f,new MO(a),(fo(),fo(),go));a.d=F(NO,m,60,[a.f])}
function OO(){rK();var a,b,c,d,e,f;OK.call(this,(fL(),gL),null,!0);this.xg();this.cb=!0;a=new GO(this.j);this.e=new HO;this.e.ob.style[Bg]=cb;Wr(this.e,cb);this.vg();iK(this,"400px");f=new JO;f.ob.style[be]=cb;f.e[Qc]=10;c=(Ht(),It);f.a=c;IO(f,a);IO(f,this.e);e=new Wt;e.e[Qc]=20;for(b=this.d,c=0,d=b.length;c<d;++c)a=b[c],Tt(e,a);IO(f,e);wK(this,f);rJ(this,!1);this.wg()}r(609,610,pF,OO);_.vg=function(){LO(this)};_.wg=function(){var a=this.e;a.ob.readOnly=!0;var b=Zr(a.ob)+"-readonly";Vr(a.qd(),b,!0)};
_.xg=function(){qJ(this.H.b,"Copy")};_.d=null;_.e=null;_.f=null;_.i="Close";_.j="Press Ctrl-C (Command-C on Mac) or right click (Option-click on Mac) on the selected text to copy it, then paste into another program.";function MO(a){this.a=a}r(612,1,{},MO);_.Uc=function(){yK(this.a,!1)};_.a=null;function PO(a){this.a=a}r(613,1,{},PO);
_.Cc=function(){ds(this.a.e.ob,!0);this.a.e.ob.focus();var a=this.a.e,b;b=vm(a.ob,tg).length;if(0<b&&a.jb){if(0>b)throw new eC("Length must be a positive integer. Length: "+b);if(b>vm(a.ob,tg).length)throw new eC("From Index: 0  To Index: "+b+"  Text Length: "+vm(a.ob,tg).length);try{a.ob.setSelectionRange(0,0+b)}catch(c){}}};_.a=null;function QO(a){a.i="Cancel";a.j="Paste the text to import into the text area below.";a.b="Accept";qJ(a.H.b,"Paste")}function RO(a){rK();OO.call(this);this.c=a}
r(615,609,pF,RO);_.vg=function(){LO(this);this.a=(new iJ(this.b)).mc.Fe();hs(this.a,new SO(this),(fo(),fo(),go));this.d=F(NO,m,60,[this.a,this.f])};_.wg=function(){Wr(this.e,"150px")};_.xg=function(){QO(this)};_.Qd=function(){NK(this);jm((gm(),hm),new TO(this))};_.a=null;_.b=null;_.c=null;function UO(a){rK();RO.call(this,a)}r(614,615,pF,UO);
_.wg=function(){Wr(this.e,"150px");var a=new VO(this),b=this.e;FO(b,new WO,(vO(),vO(),wO));FO(b,new XO,(rO(),rO(),sO));FO(b,new YO,(yO(),yO(),zO));FO(b,new ZO(a),(BO(),BO(),CO))};_.xg=function(){QO(this);this.j+=" Or drag and drop a file on it."};r(618,1,{});r(617,618,{});_.b=null;_.c=1;_.d=-1;function VO(a){this.a=a;this.b=new $O(this);this.c=this.d=1}r(616,617,{},VO);_.a=null;function $O(a){this.a=a}r(619,1,{},$O);_.yg=function(a){this.a.a.e.ob[tg]=null!=a?a:l};_.a=null;
function SO(a){this.a=a}r(621,1,{},SO);_.Uc=function(){if(this.a.c){var a=this.a.c,b;b=new Dx(a.a,0,vm(this.a.e.ob,tg));UC(a.a.a,b.a)}yK(this.a,!1)};_.a=null;function TO(a){this.a=a}r(622,1,{},TO);_.Cc=function(){ds(this.a.e.ob,!0);this.a.e.ob.focus()};_.a=null;r(623,1,ch);_.Lc=function(){var a,b;a=new aP(this.a);void 0!=$wnd.FileReader?b=new UO(a):b=new RO(a);kK(b);KO(b)};function aP(a){this.a=a}r(624,1,{},aP);_.a=null;r(625,1,ch);
_.Lc=function(){var a;a=new OO;var b=this.a,c;Mu(a.e,b);b=(c=jC(b,"\r\n|\r|\n|\n\r"),c.length);Wr(a.e,20*(10>b?b:10)+qf);jm((gm(),hm),new PO(a));kK(a);KO(a)};function EO(a,b){a.onloadend=function(a){b.yg(a.target.result)}}function ZO(a){this.a=a}r(630,1,{},ZO);_.a=null;function WO(){}r(631,1,{},WO);function XO(){}r(632,1,{},XO);function YO(){}r(633,1,{},YO);Y(618);Y(617);Y(630);Y(631);Y(632);Y(633);Y(200);Y(202);Y(201);Y(203);Y(204);Y(609);var NO=SB(770,pN);Y(615);Y(614);Y(624);Y(612);Y(613);Y(621);
Y(622);Y(616);Y(619);Y(321);Y(370);Y(361);y(iF)(5);
var KN="3",LN="Any",MN="Aromatic",NN="Nonring",ON="Reset",PN="Ring";function QN(){QN=s;RN=new jo(Uc,new SN)}function SN(){}r(196,193,{},SN);_.Tc=function(a){Tv();pK(this,a.b,TN(a.a.a,a.a.a.ob.selectedIndex))};_.Wc=function(){return RN};var RN;function UN(a,b){if(0>b||b>=a.ob.options.length)throw new Ks;}function TN(a,b){UN(a,b);return a.ob.options[b].value}function VN(){Ot();this.ob=$doc.createElement("select");this.ob[Wc]="gwt-ListBox"}r(341,319,ih,VN);function WN(){WN=s}
function XN(a,b){if(null==b)throw new Lp("Missing message: awt.103");var c=-1,d,e,f;f=a.mc.a.ob;e=$doc.createElement(kf);e.text=b;e.removeAttribute("bidiwrapped");e.value=b;d=f.options.length;(0>c||c>d)&&(c=d);c==d?f.add(e,null):(c=f.options[c],f.add(e,c))}function YN(){WN();Sv.call(this);new pi;this.mc=new ZN((Tv(),this))}r(408,395,{89:1,91:1,98:1,110:1,116:1},YN);_.be=function(){return Xv(this.mc,this)};
_.qe=function(){return(null==this.jc&&(this.jc=Ev(this)),this.jc)+va+this.uc+va+this.vc+va+this.rc+Ig+this.hc+(this.qc?l:",hidden")+",current="+TN(this.mc.a,this.mc.a.ob.selectedIndex)};function $N(){PJ.call(this,7)}r(421,1,lh,$N);function aO(a){RJ.call(this,a,0)}r(426,395,zh,aO);function bO(a){var b=a.j;oK(a.mc.c,b.a,b.b);!Gv(a)&&jJ(a);eJ(a)}
function cO(a,b,c){lK.call(this);this.mc&&KJ(this.mc.c,!1);HJ(this,!1);fw(this,new PJ(0));a=new RJ(a,1);$(this,a,null);a=new kw;$(a,this.i,null);$(this,a,null);b&&(this.j=Hv(b),GJ(this),kK(this.j,~~(Em(b.$b.ob,gf)/2)-~~(this.rc/2),~~(Em(b.$b.ob,ff)/2)-~~(this.hc/2)));c&&Z(this,c)}r(543,544,IF,cO);_.ig=function(){return"OK"};r(549,550,Eh);_.Pc=function(){bO(new cO(this.b,this.a,(Jy(),Ly)))};r(552,550,Eh);_.Pc=function(){!this.a.Ib?this.a.Ib=new dO(this.a):this.a.Ib.mc.c.gb?cL(this.a.Ib.mc.c):bO(this.a.Ib)};
function eO(a,b){aJ(b)==a.a?Z(b,(Cw(),Lw)):Z(b,a.a)}
function fO(a){var b,c,d,e;e=l;d=!1;aJ(gO)!=a.a?(e=ta,d=!0):aJ(hO)!=a.a?(e="!#6",d=!0):aJ(iO)!=a.a?(Z(jO,(Cw(),Lw)),Z(kO,Lw),Z(lO,Lw),Z(mO,Lw),e="F,Cl,Br,I"):(b=aJ(nO)!=a.a,c=aJ(oO)!=a.a,aJ(pO)!=a.a&&(b?e+="c,":c?e+="C,":e+="#6,"),aJ(qO)!=a.a&&(b?e+="n,":c?e+="N,":e+="#7,"),aJ(rO)!=a.a&&(b?e+="o,":c?e+="O,":e+="#8,"),aJ(sO)!=a.a&&(b?e+="s,":c?e+="S,":e+="#16,"),aJ(vO)!=a.a&&(b?e+="p,":c?e+="P,":e+="#15,"),aJ(jO)!=a.a&&(e+="F,"),aJ(kO)!=a.a&&(e+="Cl,"),aJ(lO)!=a.a&&(e+="Br,"),aJ(mO)!=a.a&&(e+="I,"),
EC(e,va)&&(e=e.substr(0,e.length-1-0)),1>e.length&&!a.b&&(b?e=mc:c?e=pb:(Z(gO,(Cw(),Lw)),e=ta)));b=l;d&&aJ(nO)!=a.a&&(b+=";a");d&&aJ(oO)!=a.a&&(b+=";A");aJ(wO)!=a.a&&(b+=";R");aJ(xO)!=a.a&&(b+=";!R");aJ(gO)!=a.a&&0<b.length?e=b.substr(1,b.length-1):e+=b;d=yO.mc.a.ob.selectedIndex;0<d&&(--d,e+=";H"+d);d=zO.mc.a.ob.selectedIndex;0<d&&(--d,e+=";D"+d);aJ(AO)!=a.a&&(e="~");aJ(BO)!=a.a&&(e=fb);aJ(CO)!=a.a&&(e=ob);aJ(DO)!=a.a&&(e="!@");Ix(a.e.mc,e)}
function EO(a){FO(a);GO(a);var b=yO.mc.a;UN(b,0);b.ob.options[0].selected=!0;b=zO.mc.a;UN(b,0);b.ob.options[0].selected=!0;Z(nO,a.a);Z(oO,a.a);Z(wO,a.a);Z(xO,a.a);Z(yO,a.a);Z(zO,a.a);HO(a)}function FO(a){Z(pO,a.a);Z(qO,a.a);Z(rO,a.a);Z(sO,a.a);Z(vO,a.a);Z(jO,a.a);Z(kO,a.a);Z(lO,a.a);Z(mO,a.a)}function GO(a){Z(gO,a.a);Z(hO,a.a);Z(iO,a.a)}function HO(a){Z(AO,a.a);Z(BO,a.a);Z(CO,a.a);Z(DO,a.a);a.b=!1}
function dO(a){IJ.call(this,"Atom/Bond Query");this.i=new BJ(this.ig());vw(this.q,new mK(this));this.a=(Jy(),Ly);this.c=a;this.d||(a=Hv(a),this.d=new TJ(a),kK(this.d,-150,10));this.j=this.d;fw(this,new $N);Z(this,this.a);a=new kw;fw(a,new bx(0,3,1));$(a,new aO("Atom type :"),null);gO=new BJ(LN);hO=new BJ("Any except C");iO=new BJ("Halogen");$(a,gO,null);$(a,hO,null);$(a,iO,null);$(this,a,null);a=new kw;fw(a,new bx(0,3,1));$(a,new RJ("Or select one or more from the list :",0),null);$(this,a,null);
a=new kw;fw(a,new bx(0,3,1));pO=new BJ(ub);qO=new BJ(Pb);rO=new BJ(Tb);sO=new BJ($b);vO=new BJ(Ub);jO=new BJ(Cb);kO=new BJ(yb);lO=new BJ(tb);mO=new BJ(Ib);$(a,pO,null);$(a,qO,null);$(a,rO,null);$(a,sO,null);$(a,vO,null);$(a,jO,null);$(a,kO,null);$(a,lO,null);$(a,mO,null);$(this,a,null);a=new kw;fw(a,new bx(0,3,1));yO=new YN;XN(yO,LN);XN(yO,Ya);XN(yO,$a);XN(yO,eb);XN(yO,KN);$(a,new aO("Number of hydrogens :  "),null);$(a,yO,null);$(this,a,null);a=new kw;fw(a,new bx(0,3,1));zO=new YN;XN(zO,LN);XN(zO,
Ya);XN(zO,$a);XN(zO,eb);XN(zO,KN);XN(zO,"4");XN(zO,"5");XN(zO,"6");$(a,new RJ("Number of connections :",0),null);$(a,zO,null);$(a,new RJ(" (H's don't count.)",0),null);$(this,a,null);a=new kw;fw(a,new bx(0,3,1));$(a,new aO("Atom is :"),null);nO=new BJ(MN);$(a,nO,null);oO=new BJ("Nonaromatic");$(a,oO,null);wO=new BJ(PN);$(a,wO,null);xO=new BJ(NN);$(a,xO,null);$(this,a,null);a=new kw;Z(a,Sw(aJ(this)));fw(a,new bx(0,3,1));$(a,new aO("Bond is :"),null);AO=new BJ(LN);$(a,AO,null);BO=new BJ(MN);$(a,BO,
null);CO=new BJ(PN);$(a,CO,null);DO=new BJ(NN);$(a,DO,null);$(this,a,null);a=new kw;fw(a,new bx(1,3,1));this.e=new Hx(ta,20);$(a,this.e,null);$(a,new BJ(ON),null);$(a,this.i,null);$(this,a,null);this.mc&&KJ(this.mc.c,!1);HJ(this,!1);FO(this);GO(this);HO(this);Z(nO,this.a);Z(oO,this.a);Z(wO,this.a);Z(xO,this.a);Z(yO,this.a);Z(zO,this.a);eO(this,gO);GJ(this);a=this.j;oK(this.mc.c,a.a,a.b);!Gv(this)&&jJ(this);eJ(this)}r(562,544,IF,dO);
_.jg=function(a,b){var c;H(b,ON)?(EO(this),eO(this,gO),fO(this)):E(a.f,88)?(HO(this),sq(a.f)===sq(gO)?(FO(this),GO(this)):sq(a.f)===sq(hO)?(FO(this),GO(this)):sq(a.f)===sq(iO)?(FO(this),GO(this)):sq(a.f)===sq(wO)?Z(xO,this.a):sq(a.f)===sq(xO)?(Z(wO,this.a),Z(nO,this.a)):sq(a.f)===sq(nO)?(Z(oO,this.a),Z(xO,this.a)):sq(a.f)===sq(oO)?Z(nO,this.a):sq(a.f)===sq(AO)||sq(a.f)===sq(BO)||sq(a.f)===sq(CO)||sq(a.f)===sq(DO)?(EO(this),this.b=!0):GO(this),eO(this,a.f),fO(this)):E(a.f,89)&&(HO(this),c=a.f,0==c.mc.a.ob.selectedIndex?
Z(c,this.a):Z(c,(Cw(),Lw)),fO(this));107!=this.c.e&&(this.c.e=107,pw(this.c));return!0};_.kg=function(){return Fm(this.e.mc.a.ob,Cg)};_.lg=function(){return this.b};_.b=!1;_.c=null;_.d=null;var gO=_.e=null,AO=null,hO=null,nO=null,BO=null,lO=null,pO=null,zO=null,yO=null,kO=null,jO=null,iO=null,mO=null,qO=null,oO=null,xO=null,DO=null,rO=null,vO=null,wO=null,CO=null,sO=null;function ZN(a){OE();QE.call(this);this.a=new VN;rs(this.a,new IO(this,a),(QN(),QN(),RN))}r(608,606,{},ZN);_.Ke=function(){return this.a};
_.a=null;function IO(a,b){this.a=a;this.b=b}r(609,1,{},IO);_.a=null;_.b=null;Y(543);Y(562);Y(408);Y(608);Y(609);Y(341);Y(196);y(EF)(1);
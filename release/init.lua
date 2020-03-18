-- Define some common quantities for use in Lua

	uo=PI*4.e-7;           -- magnetic permeability of free space
	eo=8.85418781762e-12;  -- electrical permittivity of free space
	Pi=PI; pi=PI;          -- various ways that one might possibly denote PI

-- Useful unit definitions

	meter=1;
	meters=meter; Meter=meter; Meters=meter;

	inch=0.0254;
	inches=inch; Inch=inch; Inches=inch; in=inch;

	cm=0.01;
	centimeter=cm; centimeters=cm; Centimeters=cm; Centimeter=cm;

	mm=0.001;
	millimeters=mm; millimeter=mm; Millimeter=mm; Millimeters=mm; milimeter=mm; milimeters=mm;

	um=0.000001;
	micrometer=um; Micrometer=um; micrometers=um; Micrometers=um;
	micron=um; Micron=um; microns=um; Microns=um;

	mil=0.001*inch;
	mils=mil; Mils=mil; Mil=mil;

	Tesla     = 1;
	mTesla    = 0.001;
	Gauss     = 0.0001;
	kGauss    = 0.1;
	AmpMeter  = 1;
	kAmpMeter = 1000;
	Oersted   = 250/PI;
	kOersted  = 1000*Oersted;

-- Useful functions

	-- Function to determine the diameter in mm of a specified AWG wire gauge
	function AWG(awg) return 8.2514694*exp(-0.115943*awg) end

	-- Function to read in BH points for a material from a file
	-- Contributed by Dr. Henning Kiel
	function mi_setbhpointsfromfile(filename, materialname, unitb, unith, order)
		fact_b = Tesla
		fact_h = AmpMeter
		bh = 1

		if unitb ~= nil then
			fact_b = unitb
		end
		if unith ~= nil then
			fact_h = unith
		end
		if order == 1 then
			bh = 0
		end
    
		bhfile = openfile(filename,"r")
		if  bhfile ~= nil then
			mi_clearbhpoints(materialname)
			if bh == 1 then
			b, h = read(bhfile,"*n","*n")
		else
			h, b = read(bhfile,"*n","*n")
		end
			while b ~= nil do
				mi_addbhpoint(materialname, b, h)
				if bh == 1 then
					b, h = read(bhfile,"*n","*n")
				else
					h, b = read(bhfile,"*n","*n")
				end
			end
			closefile(bhfile)
		end
	end


	
--permeability tables used to create asymptotic boundary conditions
		uAx0 = {{10.06344410876133}, 
				{0.18870625462846807, 39.997500411566335}, 
				{4.374794457961015, 0.07511795918154143, 85.85566438727763}, 
				{0.28509610532711227, 12.493269046515916, 0.051874008856059424, 147.65207370446487}, 
				{3.0607674892527865, 0.11237818729856126, 22.026456394011774, 0.042316694272791736, 225.43020812455183}, 
				{0.3748159380792134, 8.172423914252851, 0.07400271964825651, 33.01915762825664, 0.037123381446484645, 319.2335657378115}, 
				{2.4089242666740582, 0.14929929105743517, 13.817592213008757, 0.05821722458526954, 45.655954531895645, 0.03386729523177279, 429.0464280099345}, 
				{0.4577193805314985, 6.152383059834071, 0.09519535460494778, 19.79502033933107, 0.04958213970480729, 60.00201778458946, 0.031639182082296755, 554.8893696637082}, 
				{2.0187133623041955, 0.1872419300750821, 10.322990689266506, 0.07322869764123259, 26.290072452745143, 0.0441395144643266, 76.08450824714102, 0.030019516604331355, 696.7499046385117}, 
				{0.5332409067449392, 4.931200653946399, 0.11642450959044752, 14.522111149156393, 0.0612363797098224, 33.37497351570902, 0.04039926157266519, 93.91981759395829, 0.028790057140879656, 854.6396765772441},
				{1.7619160942694887, 0.2265234822569949, 8.305694616317721, 0.08811132253969434, 18.911084918162924, 0.053672710920365976, 41.08415574149624, 0.03767250616088464, 113.51592772683063, 0.02782511889284403, 1028.551339640375},
				{0.6010640087773658, 4.100907065931257, 0.13795163994918203, 11.604922157909524, 0.07274062063786199, 23.5602014704398, 0.048468764953025635, 49.437385787628045, 0.035597711447375925, 134.87895858524496, 0.02704793878013551, 1218.4906640339432}};
		u2D0 = {{10.523809523809524}, 
				{0.14531359011819187, 32.14290974079077}, 
				{5.14939698507243, 0.053268993904189134, 54.10636468483519}, 
				{0.2419609512179885, 13.681253040072905, 0.03301443572330777, 76.07498920312688}, 
				{3.4661091134486472, 0.09209257399023746, 21.797339506447933, 0.02406322785653838, 98.05449854377491}, 
				{0.3336928992521675, 9.023667719158183, 0.05789695118959376, 29.573300006204246, 0.018966499540708268, 120.04039743815603}, 
				{2.649899689611667, 0.12953316809300527, 14.398967120915913, 0.04288257435633827, 37.197476986533054, 0.01566424606410637, 142.03015659052124}, 
				{0.4194212714207981, 6.739646495379991, 0.0807327554079406, 19.40925290947297, 0.034255287061558344, 44.73740232258131, 0.013346899201001426, 164.02239651319712}, 
				{2.1751715650719827, 0.16743379223593088, 10.885675868734184, 0.05987838745046158, 24.237375183864483, 0.028600398122246736, 52.225223074983184, 0.0116295354236669, 186.01632084646832}, 
				{0.4982294821100038, 5.3544134782776664, 0.10291820111116401, 14.696545405270147, 0.048003097591685244, 28.95632726953973, 0.024587012459820066, 59.67849241525681, 0.010305182059498453, 208.01143556937637},
				{1.8696806205952006, 0.2063934470334104, 8.775890039744148, 0.07607619682670601, 18.328256642424236, 0.04023921548166901, 33.60353647639198, 0.021581843440607016, 67.10765452494896, 0.009252450549701629, 230.0074236348898},
				{0.5695628007801712, 4.41799489542221, 0.12499429930905502, 11.902080658968998, 0.06098009575496433, 21.847988015751035, 0.03472775671050834, 38.20087849238854, 0.01924286302232932, 74.51932204012905, 0.00839537165467591, 252.00407064779833}};
		uAx1 = {{0.09090909090909091}, 
				{10.050515900211503, 0.04349612665401649}, 
				{0.16015012096541084, 31.212316587655547, 0.035617897941640685}, 
				{5.075223589959199, 0.06747975164123507, 58.83076738028796, 0.032496412704901965}, 
				{0.24983741484804536, 13.959519296714365, 0.04850942160789329, 93.62679411332631, 0.030834135426565015}, 
				{3.438673996192938, 0.10007632616355028, 23.68319910391683, 0.040588823403447945, 135.81886600712073, 0.029800226566381535}, 
				{0.33861114976053774, 9.11830816329029, 0.06748841981310061, 34.53152988922068, 0.03624009732510784, 185.4917052298402, 0.02909733301094425}, 
				{2.6364338943965455, 0.13486508168984512, 15.097445398169498, 0.05395605788825772, 46.75319306175651, 0.03349209595847733, 242.6651438041285, 0.028587355212278894}, 
				{0.4227311219273812, 6.773013674320017, 0.08745530894064872, 21.301429520550652, 0.04651420711694375, 60.44151623434645, 0.03160001126426384, 307.3631853353253, 0.028201318378415573}, 
				{2.1675591338727647, 0.171316009911807, 11.198891625678383, 0.0680375716016307, 27.948228063433724, 0.041804487302954434, 75.63799976250523, 0.030217675069207188, 379.586805599126, 0.027898435150547626},
				{0.5005548322806631, 5.365891158623955, 0.1079474936583888, 15.594898520264833, 0.05739172309462815, 35.12584682017268, 0.03855673261275498, 92.36682653198929, 0.029164062639383995, 459.3468429136272, 0.027654812310257123},
				{1.864938491614021, 0.20938145992414733, 8.933546101982833, 0.08240945574778373, 20.142380617987286, 0.05066029544301638, 42.87550155678245, 0.036182409831124134, 110.63472923382548, 0.0283338992149914, 546.6150102543896, 0.027453323354323905}};
		u2D1 = {{0.09502262443438914}, 
				{6.88166880459455, 0.031111060201589526}, 
				{0.19419749592018187, 18.77264665066931, 0.018482113995736214},
				{4.13289828365353, 0.07309272016758717, 30.28978015498877, 0.013144924639159823}, 
				{0.28850793996067764, 10.858638831248303, 0.04587715852680953, 41.55718451247941, 0.010198410219328852}, 
				{2.9967673937356176, 0.1108196834283798, 17.27206665382645, 0.0338142851758244, 52.724541914214335, 0.008330528899783033}, 
				{0.37737277525645235, 7.720030434644211, 0.0694494258951436, 23.31949550539958, 0.026883543752277105, 63.83965087550882, 0.0070407582729502555}, 
				{2.384237702546398, 0.14837573304995255, 12.386546277094604, 0.05152181760881001, 29.192574288770942, 0.022352660997929614, 74.923770274119, 0.006096728349565376}, 
				{0.45973385090978197, 5.972510009155741, 0.09186384125878651, 16.700516539917132, 0.04125859307841753, 34.96454824599637, 0.019147835875477914, 85.98795769304176, 0.005375872372109578}, 
				{2.007107238545973, 0.1867618188354154, 9.71645432200938, 0.06804320147518493, 20.83198897925314, 0.0345347664671527, 40.67187917337226, 0.01675645545872318, 97.03855732255441, 0.004807428001555607},
				{0.5348507060428623, 4.845115067234294, 0.11394855626850516, 13.144715978348659, 0.05456056293348216, 24.85137913425649, 0.029758772583431677, 46.33524484374972, 0.014901429756097718, 108.07947522964582, 0.004347685758123113},
				{1.755732640246568, 0.22634702476369292, 8.00036486086015, 0.08401892313227052, 16.39879353450492, 0.04577080504067754, 28.795410205618264, 0.026177408464552673, 51.96731893999375, 0.013419338402750022, 119.11324967287628, 0.003968189868637491}};
				
function mi_drawarc(x1,y1,x2,y2,tta,dtta)
	mi_addnode(x1,y1);
	mi_addnode(x2,y2);
	mi_addarc(x1,y1,x2,y2,tta,dtta);
end

function mi_drawline(x1,y1,x2,y2)
	mi_addnode(x1,y1);
	mi_addnode(x2,y2);
	mi_addsegment(x1,y1,x2,y2);
end

function mi_makeABC(enn,arr,ex,wye,bc)

	local k, d, dp, z, r, x0, x1, y0, y1, flag, p1, p2, p3, x, y, R, n
	
	x0, x1, y0, y1 = mi_getboundingbox();
	flag, p1, p2, p3 = mi_getprobleminfo();
		
	-- unpack parameters;
	if (enn == nil) then
		n=7;
	else
		if (enn>12) then
			n=12;
		elseif (enn<1) then
			n=1;
		else
			n=enn;
		end
	end
	
	if (bc == nil) then
		bctype=0;
	else
		bctype=bc;
	end
	
	if(flag == 0) then -- 2D planar case
	
		if (arr == nil) then
			R=(3/4)*abs((x0+I*y0)-(x1+I*y1));
		else
			R = arr;
		end
		
		if (ex == nil) then
			x=(x0+x1)/2;
		else
			x=ex;
		end
		
		if (wye == nil) then
			y=(y0+y1)/2;
		else	
			y=wye;
		end
		
    else -- Axi case
		x=0;
		if (wye ~= nil) then
			y=wye;
			R=arr;
		elseif (ex ~= nil) then 
			y=ex;
			R=arr;
		elseif (arr ~= nil) then
			y=(y0+y1)/2;
			R=arr;
		else
			y=(y0+y1)/2;
			R=(3/2)*abs(x1+I*(y1-y0)/2);
		end
	end
	
	-- draw left boundary of interior domain
	if (flag == 0) then 
		mi_drawarc(x, y + R, x, y - R, 180, 1);
	else
		mi_drawline(0, y-1.1*R, 0, y+1.1*R);
	end
	
	-- draw right boundary of interior domain
	mi_drawarc(x, y - R, x, y + R, 180, 1);
	
    d = 0.1*R/(2*n);
    
	for k = 1,n do
		r = R*(1 + (2*k - 1)/(20*n));
		mi_drawarc(x, y - r - d, x, y + r + d, 180, 1);
		z = r*exp(I*(90/(n+1))*k*Pi/180);
		mi_addblocklabel(x+re(z),y+im(z));
		mi_selectlabel(x+re(z),y+im(z));
		mi_setblockprop("u" .. k, 1, 0, "<None>", 0, 0, 1);
		mi_clearselected();
		if(flag == 0) then
			if (bctype==0) then
				mi_addmaterial("u" .. k, u2D0[n][k]);
			else
				mi_addmaterial("u" .. k, u2D1[n][k]);
			end
			mi_drawarc(x, y + r + d, x, y - r - d, 180, 1);
		else
			if (bctype==0) then
				mi_addmaterial("u" .. k, uAx0[n][k]);
			else
				mi_addmaterial("u" .. k, uAx1[n][k]);
			end
		end
    end
	
	if (bctype==0) then
		mi_addboundprop("A=0", 0, 0, 0, 0, 0, 0, 0, 0, 0);
		mi_selectarcsegment(1.1*R+x, y);
		if(flag == 0) then
			mi_selectarcsegment(-1.1*R+x, y);
		end
		mi_setarcsegmentprop(1, "A=0", 0, 0);
		mi_clearselected();
	end
	
	mi_zoomnatural();
end

--functions used to create asymptotic boundary conditions for electrostatic problems
	
function ei_drawarc(x1,y1,x2,y2,tta,dtta)
	ei_addnode(x1,y1);
	ei_addnode(x2,y2);
	ei_addarc(x1,y1,x2,y2,tta,dtta);
end

function ei_drawline(x1,y1,x2,y2)
	ei_addnode(x1,y1);
	ei_addnode(x2,y2);
	ei_addsegment(x1,y1,x2,y2);
end

function ei_makeABC(enn,arr,ex,wye,bc)

	local k, d, dp, z, r, x0, x1, y0, y1, flag, p1, p2, p3, x, y, R
	
	x0, x1, y0, y1 = ei_getboundingbox();
	flag, p1, p2, p3 = ei_getprobleminfo();
	
	-- unpack parameters;
	if (enn == nil) then
		n=7;
	else
		if (enn>12) then
			n=12;
		elseif (enn<1) then
			n=1;
		else
			n=enn;
		end
	end
	
	if (bc == nil) then
		if (flag == 0) then
			-- if 2D, use Neumann-type BC by default
			bctype=1;
		else 
			-- if Axisymmetric, use Dirichlet-type BC by default
			bctype=0;
		end
	else
		bctype=bc;
	end
	
	if(flag == 0) then -- 2D planar case
	
		if (arr == nil) then
			R=(3/4)*abs((x0+I*y0)-(x1+I*y1));
		else
			R = arr;
		end
		
		if (ex == nil) then
			x=(x0+x1)/2;
		else
			x=ex;
		end
		
		if (wye == nil) then
			y=(y0+y1)/2;
		else	
			y=wye;
		end
		
    else -- Axi case
		x=0;
		if (wye ~= nil) then
			y=wye;
			R=arr;
		elseif (ex ~= nil) then 
			y=ex;
			R=arr;
		elseif (arr ~= nil) then
			y=(y0+y1)/2;
			R=arr;
		else
			y=(y0+y1)/2;
			R=(3/2)*abs(x1+I*(y1-y0)/2);
		end
	end
	
	-- draw left boundary of interior domain
	if (flag == 0) then 
		ei_drawarc(x, y + R, x, y - R, 180, 1);
	else
		ei_drawline(0, y-1.1*R, 0, y+1.1*R);
	end
	
	-- draw right boundary of interior domain
	ei_drawarc(x, y - R, x, y + R, 180, 1);
	
    d = 0.1*R/(2*n);
    
	for k = 1,n do
		r = R*(1 + (2*k - 1)/(20*n));
		ei_drawarc(x, y - r - d, x, y + r + d, 180, 1);
		z = r*exp(I*(90/(n+1))*k*Pi/180);
		ei_addblocklabel(x+re(z),y+im(z));
		ei_selectlabel(x+re(z),y+im(z));
		ei_setblockprop("e" .. k, 1, 0, "<None>", 0, 0, 1);
		ei_clearselected();
		if(flag == 0) then
			if (bctype==0) then
				ei_addmaterial("e" .. k, u2D1[n][k]);
			else
				ei_addmaterial("e" .. k, u2D0[n][k]);
			end
			ei_drawarc(x, y + r + d, x, y - r - d, 180, 1);
		else
			if (bctype==0) then
				ei_addmaterial("e" .. k, uAx1[n][k]);
			else
				ei_addmaterial("e" .. k, uAx0[n][k]);
			end
		end
    end
	
	if (bctype==0) then
		ei_addboundprop("V=0", 0, 0, 0, 0, 0, 0, 0, 0, 0);
		ei_selectarcsegment(1.1*R+x, y);
		if(flag == 0) then
			ei_selectarcsegment(-1.1*R+x, y);
		end
		ei_setarcsegmentprop(1, "V=0", 0, 0);
		ei_clearselected();
	end
	
	ei_zoomnatural();
end

--functions used to create asymptotic boundary conditions for current flow problems
	
function ci_drawarc(x1,y1,x2,y2,tta,dtta)
	ci_addnode(x1,y1);
	ci_addnode(x2,y2);
	ci_addarc(x1,y1,x2,y2,tta,dtta);
end

function ci_drawline(x1,y1,x2,y2)
	ci_addnode(x1,y1);
	ci_addnode(x2,y2);
	ci_addsegment(x1,y1,x2,y2);
end

function ci_makeABC(enn,arr,ex,wye,bc)

	local k, d, dp, z, r, x0, x1, y0, y1, flag, p1, p2, p3, x, y, R
	
	x0, x1, y0, y1 = ci_getboundingbox();
	flag, p1, p2, p3 = ci_getprobleminfo();
	
	-- unpack parameters;
	if (enn == nil) then
		n=7;
	else
		if (enn>12) then
			n=12;
		elseif (enn<1) then
			n=1;
		else
			n=enn;
		end
	end
	
	if (bc == nil) then
		if (flag == 0) then
			-- if 2D, use Neumann-type BC by default
			bctype=1;
		else 
			-- if Axisymmetric, use Dirichlet-type BC by default
			bctype=0;
		end
	else
		bctype=bc;
	end
	
	if(flag == 0) then -- 2D planar case
	
		if (arr == nil) then
			R=(3/4)*abs((x0+I*y0)-(x1+I*y1));
		else
			R = arr;
		end
		
		if (ex == nil) then
			x=(x0+x1)/2;
		else
			x=ex;
		end
		
		if (wye == nil) then
			y=(y0+y1)/2;
		else	
			y=wye;
		end
		
    else -- Axi case
		x=0;
		if (wye ~= nil) then
			y=wye;
			R=arr;
		elseif (ex ~= nil) then 
			y=ex;
			R=arr;
		elseif (arr ~= nil) then
			y=(y0+y1)/2;
			R=arr;
		else
			y=(y0+y1)/2;
			R=(3/2)*abs(x1+I*(y1-y0)/2);
		end
	end
	
	-- draw left boundary of interior domain
	if (flag == 0) then 
		ci_drawarc(x, y + R, x, y - R, 180, 1);
	else
		ci_drawline(0, y-1.1*R, 0, y+1.1*R);
	end
	
	-- draw right boundary of interior domain
	ci_drawarc(x, y - R, x, y + R, 180, 1);
	
    d = 0.1*R/(2*n);
    
	for k = 1,n do
		r = R*(1 + (2*k - 1)/(20*n));
		ci_drawarc(x, y - r - d, x, y + r + d, 180, 1);
		z = r*exp(I*(90/(n+1))*k*Pi/180);
		ci_addblocklabel(x+re(z),y+im(z));
		ci_selectlabel(x+re(z),y+im(z));
		ci_setblockprop("e" .. k, 1, 0, "<None>", 0, 0, 1);
		ci_clearselected();
		if(flag == 0) then
			if (bctype==0) then
				ci_addmaterial("e" .. k, u2D1[n][k]);
			else
				ci_addmaterial("e" .. k, u2D0[n][k]);
			end
			ci_drawarc(x, y + r + d, x, y - r - d, 180, 1);
		else
			if (bctype==0) then
				ci_addmaterial("e" .. k, uAx1[n][k]);
			else
				ci_addmaterial("e" .. k, uAx0[n][k]);
			end
		end
    end
	
	if (bctype==0) then
		ci_addboundprop("V=0", 0, 0, 0, 0, 0, 0, 0, 0, 0);
		ci_selectarcsegment(1.1*R+x, y);
		if(flag == 0) then
			ci_selectarcsegment(-1.1*R+x, y);
		end
		ci_setarcsegmentprop(1, "V=0", 0, 0);
		ci_clearselected();
	end
	
	ci_zoomnatural();
end

--functions used to create asymptotic boundary conditions for heat flow problems
	
function hi_drawarc(x1,y1,x2,y2,tta,dtta)
	hi_addnode(x1,y1);
	hi_addnode(x2,y2);
	hi_addarc(x1,y1,x2,y2,tta,dtta);
end

function hi_drawline(x1,y1,x2,y2)
	hi_addnode(x1,y1);
	hi_addnode(x2,y2);
	hi_addsegment(x1,y1,x2,y2);
end

function hi_makeABC(enn,arr,ex,wye,bc)

	local k, d, dp, z, r, x0, x1, y0, y1, flag, p1, p2, p3, x, y, R
	
	x0, x1, y0, y1 = hi_getboundingbox();
	flag, p1, p2, p3 = hi_getprobleminfo();

	-- unpack parameters;
	if (enn == nil) then
		n=7;
	else
		if (enn>12) then
			n=12;
		elseif (enn<1) then
			n=1;
		else
			n=enn;
		end
	end
	
	if (bc == nil) then
		if (flag == 0) then
			-- if 2D, use Neumann-type BC by default
			bctype=1;
		else 
			-- if Axisymmetric, use Dirichlet-type BC by default
			bctype=0;
		end
	else
		bctype=bc;
	end
	
	if(flag == 0) then -- 2D planar case
	
		if (arr == nil) then
			R=(3/4)*abs((x0+I*y0)-(x1+I*y1));
		else
			R = arr;
		end
		
		if (ex == nil) then
			x=(x0+x1)/2;
		else
			x=ex;
		end
		
		if (wye == nil) then
			y=(y0+y1)/2;
		else	
			y=wye;
		end
		
    else -- Axi case
		x=0;
		if (wye ~= nil) then
			y=wye;
			R=arr;
		elseif (ex ~= nil) then 
			y=ex;
			R=arr;
		elseif (arr ~= nil) then
			y=(y0+y1)/2;
			R=arr;
		else
			y=(y0+y1)/2;
			R=(3/2)*abs(x1+I*(y1-y0)/2);
		end
	end
	
	-- draw left boundary of interior domain
	if (flag == 0) then 
		hi_drawarc(x, y + R, x, y - R, 180, 1);
	else
		hi_drawline(0, y-1.1*R, 0, y+1.1*R);
	end
	
	-- draw right boundary of interior domain
	hi_drawarc(x, y - R, x, y + R, 180, 1);
	
    d = 0.1*R/(2*n);
    
	for k = 1,n do
		r = R*(1 + (2*k - 1)/(20*n));
		hi_drawarc(x, y - r - d, x, y + r + d, 180, 1);
		z = r*exp(I*(90/(n+1))*k*Pi/180);
		hi_addblocklabel(x+re(z),y+im(z));
		hi_selectlabel(x+re(z),y+im(z));
		hi_setblockprop("e" .. k, 1, 0, "<None>", 0, 0, 1);
		hi_clearselected();
		if(flag == 0) then
			if (bctype==0) then
				hi_addmaterial("e" .. k, u2D1[n][k]);
			else
				hi_addmaterial("e" .. k, u2D0[n][k]);
			end
			hi_drawarc(x, y + r + d, x, y - r - d, 180, 1);
		else
			if (bctype==0) then
				hi_addmaterial("e" .. k, uAx1[n][k]);
			else
				hi_addmaterial("e" .. k, uAx0[n][k]);
			end
		end
    end
	
	if (bctype==0) then
		hi_addboundprop("V=0", 0, 0, 0, 0, 0, 0, 0, 0, 0);
		hi_selectarcsegment(1.1*R+x, y);
		if(flag == 0) then
			hi_selectarcsegment(-1.1*R+x, y);
		end
		hi_setarcsegmentprop(1, "V=0", 0, 0);
		hi_clearselected();
	end
	
	hi_zoomnatural();
end

-- convenience functions

function mi_drawrectangle(x1,y1,x2,y2)
	mi_drawline(x1,y1,x2,y1);
	mi_drawline(x2,y1,x2,y2);
	mi_drawline(x2,y2,x1,y2);
	mi_drawline(x1,y2,x1,y1);
end

function ei_drawrectangle(x1,y1,x2,y2)
	ei_drawline(x1,y1,x2,y1);
	ei_drawline(x2,y1,x2,y2);
	ei_drawline(x2,y2,x1,y2);
	ei_drawline(x1,y2,x1,y1);
end

function ci_drawrectangle(x1,y1,x2,y2)
	ci_drawline(x1,y1,x2,y1);
	ci_drawline(x2,y1,x2,y2);
	ci_drawline(x2,y2,x1,y2);
	ci_drawline(x1,y2,x1,y1);
end

function hi_drawrectangle(x1,y1,x2,y2)
	hi_drawline(x1,y1,x2,y1);
	hi_drawline(x2,y1,x2,y2);
	hi_drawline(x2,y2,x1,y2);
	hi_drawline(x1,y2,x1,y1);
end
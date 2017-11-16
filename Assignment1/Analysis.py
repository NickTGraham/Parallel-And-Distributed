from matplotlib import pyplot
import numpy as np

threads = [1,4,16,24,32,54]
increments = [1, 100, 10000, 1000000, 5000000]
oneThread = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 15, 17, 18, 19, 20, 21, 22, 23, 24]
fourThread = [x+25 for x in oneThread]
sixteenThread = [x+25 for x in fourThread]
twentyFourThread = [x+25 for x in sixteenThread]
thirtyTwoThread = [x+25 for x in twentyFourThread]
fiftyFourThread = [x+25 for x in thirtyTwoThread]

oneIter = [0, 1, 2, 3, 4]
oneHundredIter = [x+5 for x in oneIter]
tenThousandIter = [x+5 for x in oneHundredIter]
oneMillionIter = [x+5 for x in tenThousandIter]
fiveMilltionIter = [x+5 for x in oneMillionIter]

Method1 = [ [1,9.927e-05], [1,9.9778e-05], [1,9.7771e-05], [1,0.000107368], [1,6.5349e-05], [100,9.5971e-05], [100,9.6661e-05], [100,9.633e-05], [100,7.0435e-05], [100,9.7147e-05], [10000,0.000133807], [10000,0.000145624], [10000,0.000111557], [10000,0.00014251], [10000,0.000152976], [1000000,0.0048743], [1000000,0.00512776], [1000000,0.00482921], [1000000,0.00497374], [1000000,0.00487757], [5000000,0.0241327], [5000000,0.016572], [5000000,0.0202087], [5000000,0.0241719], [5000000,0.0242027], [4,9.3245e-05], [4,9.1562e-05], [4,9.507e-05], [4,9.5208e-05], [4,9.4578e-05], [336,6.4433e-05], [400,7.3645e-05], [337,6.9887e-05], [347,5.2952e-05], [340,6.7475e-05], [20644,0.000401228], [10023,0.000362628], [13038,0.000418879], [40000,0.000285239], [32009,0.000373501], [1748599,0.0304576], [1243556,0.0347162], [1531145,0.0284422], [1780329,0.0346417], [2346210,0.014933], [6232531,0.120425], [6002247,0.0982083], [8832120,0.11297], [6505855,0.12907], [6548601,0.113672], [16,0.000317122], [16,0.000457194], [15,0.000369691], [16,0.000411028], [15,0.000311474], [630,0.000327547], [104,0.000176172], [504,0.000279289], [768,0.000247858], [1600,0.000347061], [75581,0.00180312], [122713,0.00101127], [76067,0.00114648], [55840,0.00137368], [147259,0.000811604], [1932659,0.108006], [3516690,0.0806066], [2073886,0.0778906], [2126472,0.0944398], [2027953,0.0980776], [6155546,0.363387], [7988626,0.333281], [6790020,0.396346], [8235304,0.373524], [8391698,0.343266], [24,0.000547295], [24,0.000424819], [24,0.000275916], [19,0.000411049], [19,0.00053107], [2400,0.000358233], [2400,0.000517543], [218,0.000254589], [1341,0.000398631], [129,0.000185867], [14597,0.00207268], [225849,0.00103943], [83117,0.00133081], [189781,0.000945014], [230006,0.00113563], [1746777,0.127579], [2922558,0.119685], [2577490,0.0978976], [3070354,0.14215], [2360599,0.146625], [8972311,0.531181], [7305526,0.568959], [7716301,0.591063], [6753749,0.55329], [7918371,0.621854], [32,0.000376459], [32,0.000464952], [32,0.000413785], [30,0.000500269], [22,0.000473577], [2674,0.000354927], [822,0.000428704], [281,0.000255116], [193,0.000262701], [264,0.000183773], [19023,0.00216479], [14708,0.00226577], [13784,0.00216927], [11722,0.0022566], [19182,0.00212946], [2078206,0.161664], [3005428,0.193038], [2848173,0.161245], [3292629,0.169136], [2325782,0.161442], [8646946,0.877994], [17631820,0.619439], [9897443,0.854524], [17672569,0.592436], [12416654,0.719227], [51,0.000504976], [52,0.0006211], [47,0.000626898], [32,0.000774311], [26,0.000716248], [1528,0.000636945], [1072,0.000475606], [1030,0.000446463], [1013,0.000532635], [555,0.000413297], [41125,0.00378467], [45401,0.00430035], [38665,0.0039842], [55238,0.00351419], [105918,0.00421615], [2986494,0.331934], [2610278,0.359385], [2764250,0.373275], [2719655,0.346083], [3258635,0.322893], [19926644,1.55907], [20741505,1.53167], [16684432,1.6001], [15956637,1.70159], [12750269,1.7561]]

Method2 = [[1,9.6495e-05], [1,0.000113398],  [1,9.1921e-05], [1,9.6131e-05], [1,9.2247e-05], [100,9.4267e-05], [100,9.4725e-05], [100,9.3441e-05], [100,7.8921e-05], [100,8.3048e-05], [10000,0.000714919], [10000,0.000736867], [10000,0.00072642], [10000,0.000773285], [10000,0.000735103], [1000000,0.0563776], [1000000,0.0562587], [1000000,0.0558719], [1000000,0.0563604], [1000000,0.0562777], [5000000,0.178622], [5000000,0.178101], [5000000,0.178746], [5000000,0.178315], [5000000,0.178437], [4,8.6491e-05], [4,8.5078e-05], [4,8.6601e-05], [4,5.7891e-05], [4,5.8932e-05], [400,0.000103338], [400,0.000146352], [400,0.000138535], [400,0.0001567], [400,0.000112812], [40000,0.0086223], [40000,0.0115419], [40000,0.0103298], [40000,0.0124692], [40000,0.00695948], [4000000,0.652661], [4000000,0.651799], [4000000,0.495703], [4000000,0.577218], [4000000,0.616663], [20000000,2.54811], [20000000,2.69447], [20000000,2.43713], [20000000,2.63172], [20000000,2.62504], [16,0.000315188], [16,0.000281953], [16,0.000321177], [16,0.0002454], [16,0.0002563], [1600,0.00061923], [1600,0.000447152], [1600,0.000394713], [1600,0.0007017], [1600,0.000531042], [160000,0.0505154], [160000,0.0408776], [160000,0.053373], [160000,0.0478391], [160000,0.0498154], [16000000,3.19121], [16000000,3.12487], [16000000,3.25005], [16000000,3.2474], [16000000,3.217], [80000000,15.8527], [80000000,15.5732], [80000000,16.6049], [80000000,15.9036], [80000000,15.6318], [24,0.000486768], [24,0.000338815], [24,0.000479049], [24,0.000496966], [24,0.000251492], [2400,0.000618194], [2400,0.000891046], [2400,0.000899323], [2400,0.00093128], [2400,0.00105562], [240000,0.0677883], [240000,0.0588942], [240000,0.0717184], [240000,0.0676488], [240000,0.0736487], [24000000,5.09675], [24000000,5.13692], [24000000,5.19618], [24000000,5.25642], [24000000,5.40778], [120000000,25.1681], [120000000,25.0243], [120000000,24.6642], [120000000,24.4604], [120000000,27.0304], [32,0.000481874], [32,0.000478978], [32,0.000463647], [32,0.000382129], [32,0.000523943], [3200,0.00130045], [3200,0.0014994], [3200,0.00128125], [3200,0.0012909], [3200,0.00111893], [320000,0.0811181], [320000,0.0912391], [320000,0.0999315], [320000,0.0917047], [320000,0.0972149], [32000000,6.90621], [32000000,6.77226], [32000000,7.34243], [32000000,6.97703], [32000000,7.3709], [160000000,36.4239], [160000000,35.2098], [160000000,33.4448], [160000000,35.7275], [160000000,35.3147], [54,0.000982516], [54,0.000698227], [54,0.000616111], [54,0.000629327], [54,0.000627035], [5400,0.00214161], [5400,0.00191901], [5400,0.00202707], [5400,0.00186644], [5400,0.00198307], [540000,0.145889], [540000,0.127582], [540000,0.144347], [540000,0.163316], [540000,0.156984], [54000000,13.7915], [54000000,13.5661], [54000000,14.93], [54000000,13.3804], [54000000,14.1899], [270000000,74.7253], [270000000,71.2422], [270000000,71.9651], [270000000,73.2466], [270000000,68.9634]]

Method3 = [ [1,8.7588e-05], [1,9.2697e-05], [1,8.5052e-05], [1,0.000112328], [1,6.5122e-05], [100,9.3678e-05], [100,6.1641e-05], [100,9.3658e-05], [100,3.2555e-05], [100,5.7808e-05], [10000,0.000816041], [10000,0.000821908], [10000,0.000834641], [10000,0.00081875], [10000,0.000803159], [1000000,0.0459738], [1000000,0.0609848], [1000000,0.0607094], [1000000,0.0610692], [1000000,0.060469], [5000000,0.190257], [5000000,0.19044], [5000000,0.189686], [5000000,0.17616], [5000000,0.190243], [4,8.1648e-05], [4,8.1032e-05], [4,5.3599e-05], [4,0.000118468], [4,5.9312e-05], [400,0.000117537], [400,6.8217e-05], [400,0.000176543], [400,0.000133773], [400,0.000109001], [40000,0.00656632], [40000,0.0123726], [40000,0.00781634], [40000,0.0145574], [40000,0.0134943], [4000000,0.653248], [4000000,0.597713], [4000000,0.639026], [4000000,0.585508], [4000000,0.606756], [20000000,3.29299], [20000000,2.75668], [20000000,2.88471], [20000000,2.58893], [20000000,2.53611], [16,0.000336291], [16,0.000446776], [16,0.000244977], [16,0.000369582], [16,0.000374524], [1600,0.00085393], [1600,0.000524392], [1600,0.000723426], [1600,0.000727434], [1600,0.000403398], [160000,0.0520539], [160000,0.0367394], [160000,0.0436975], [160000,0.049724], [160000,0.0572921], [16000000,3.46864], [16000000,3.22053], [16000000,3.42995], [16000000,3.13235], [16000000,3.53491], [80000000,15.8975], [80000000,17.4044], [80000000,16.3565], [80000000,16.3146], [80000000,16.3061], [24,0.000414358], [24,0.000536546], [24,0.000468994], [24,0.000586141], [24,0.000438662], [2400,0.00100233], [2400,0.000599265], [2400,0.000984443], [2400,0.00114671], [2400,0.000814032], [240000,0.0899706], [240000,0.0871431], [240000,0.0755299], [240000,0.0744678], [240000,0.0855754], [24000000,5.85214], [24000000,5.55457], [24000000,5.27864], [24000000,5.7767], [24000000,5.37678], [120000000,27.2436], [120000000,26.1204], [120000000,26.2978], [120000000,28.241], [120000000,26.4882], [32,0.000545604], [32,0.000442596], [32,0.000479949], [32,0.000502135], [32,0.000494782], [3200,0.00143802], [3200,0.00129729], [3200,0.00121443], [3200,0.00123208], [3200,0.00125242], [320000,0.108988], [320000,0.0975827], [320000,0.10447], [320000,0.108065], [320000,0.105797], [32000000,7.84589], [32000000,7.96432], [32000000,7.7395], [32000000,8.22343], [32000000,7.8092], [160000000,38.4501], [160000000,38.873], [160000000,38.3284], [160000000,39.2372], [160000000,36.4846], [54,0.000508385], [54,0.000610321], [54,0.000652351], [54,0.000592554], [54,0.000723419], [5400,0.00227312], [5400,0.00208402], [5400,0.00198764], [5400,0.00200311], [5400,0.00208897], [540000,0.160088], [540000,0.182884], [540000,0.182041], [540000,0.189331], [540000,0.204953], [54000000,15.6447], [54000000,16.1103], [54000000,15.1291], [54000000,14.4779], [54000000,15.764], [270000000,75.1573], [270000000,72.9138], [270000000,75.556], [270000000,74.7023], [270000000,75.7805]]

Method4 = [ [1,8.3099e-05], [1,8.6474e-05], [1,8.5174e-05], [1,6.4911e-05], [1,5.8429e-05], [100,6.2289e-05], [100,3.8519e-05], [100,5.4709e-05], [100,2.0356e-05], [100,3.7526e-05], [10000,0.000315842], [10000,0.000296886], [10000,0.000297313], [10000,0.000268031], [10000,0.000312899], [1000000,0.0203537], [1000000,0.0211896], [1000000,0.0197019], [1000000,0.021128], [1000000,0.0210048], [5000000,0.0852327], [5000000,0.0853328], [5000000,0.0856588], [5000000,0.0854757], [5000000,0.0850884], [4,5.2749e-05], [4,5.2116e-05], [4,8.8928e-05], [4,8.1588e-05], [4,8.5181e-05], [400,9.984e-05], [400,6.009e-05], [400,9.7666e-05], [400,8.3044e-05], [400,5.6069e-05], [40000,0.000786014], [40000,0.00205907], [40000,0.00140066], [40000,0.000759084], [40000,0.000782085], [4000000,0.10009], [4000000,0.124475], [4000000,0.0946875], [4000000,0.080622], [4000000,0.082225], [20000000,0.454773], [20000000,0.410001], [20000000,0.483113], [20000000,0.452964], [20000000,0.440617], [16,0.000429006], [16,0.000349812], [16,0.000345941], [16,0.000427806], [16,0.000247081], [1600,0.000344987], [1600,0.00036431], [1600,0.000346601], [1600,0.000180766], [1600,0.000441768], [160000,0.00790111], [160000,0.0061723], [160000,0.0057922], [160000,0.00357155], [160000,0.00703442], [16000000,0.335488], [16000000,0.317642], [16000000,0.350372], [16000000,0.351638], [16000000,0.34077], [80000000,1.67395], [80000000,1.72189], [80000000,1.78502], [80000000,1.73554], [80000000,1.72568], [24,0.00040464], [24,0.000345369], [24,0.000377741], [24,0.000449683], [24,0.000480988], [2400,0.000386761], [2400,0.000377433], [2400,0.000422895], [2400,0.0002237], [2400,0.00045728], [240000,0.00742637], [240000,0.00546834], [240000,0.00548329], [240000,0.00867142], [240000,0.00691], [24000000,0.50268], [24000000,0.495605], [24000000,0.519222], [24000000,0.496865], [24000000,0.518134], [120000000,2.55605], [120000000,2.55907], [120000000,2.50607], [120000000,2.53522], [120000000,2.55356], [32,0.000458521], [32,0.000440838], [32,0.000568737], [32,0.000616846], [32,0.000444192], [3200,0.000282065], [3200,0.000284024], [3200,0.000251359], [3200,0.000259097], [3200,0.000239793], [320000,0.0132477], [320000,0.0138736], [320000,0.0131821], [320000,0.0139274], [320000,0.0131789], [32000000,0.654715], [32000000,0.661823], [32000000,0.642328], [32000000,0.661531], [32000000,0.676865], [160000000,3.42665], [160000000,3.31892], [160000000,3.42717], [160000000,3.31964], [160000000,3.34877], [54,0.000649516], [54,0.000778815], [54,0.000805905], [54,0.000755584], [54,0.000777134], [5400,0.000561568], [5400,0.000422839], [5400,0.000373667], [5400,0.000396049], [5400,0.000381659], [540000,0.017596], [540000,0.0178499], [540000,0.0170474], [540000,0.0162464], [540000,0.0182501], [54000000,1.09716], [54000000,1.08614], [54000000,1.08892], [54000000,1.09568], [54000000,1.10979], [270000000,5.58751], [270000000,5.47617], [270000000,5.52605], [270000000,5.45174], [270000000,5.31737]]

Method5 = [ [1,9.3132e-05], [1,8.3981e-05], [1,9.2794e-05], [1,2.3103e-05], [1,3.9615e-05], [100,2.5359e-05], [100,1.3894e-05], [100,1.4933e-05], [100,1.5647e-05], [100,2.6666e-05], [10000,0.000130461], [10000,0.000138414], [10000,0.00013247], [10000,7.1476e-05], [10000,0.000130144], [1000000,0.00488825], [1000000,0.00512675], [1000000,0.00511871], [1000000,0.00497276], [1000000,0.00354212], [5000000,0.0166257], [5000000,0.0240999], [5000000,0.02362], [5000000,0.0238151], [5000000,0.0242686], [4,8.1281e-05], [4,8.6221e-05], [4,8.3075e-05], [4,5.9774e-05], [4,5.4191e-05], [400,8.6808e-05], [400,4.842e-05], [400,7.1601e-05], [400,3.0993e-05], [400,6.7722e-05], [40000,0.000319892], [40000,0.000392325], [40000,0.000276622], [40000,0.000364982], [40000,0.000269757], [4000000,0.023402], [4000000,0.0249829], [4000000,0.0298088], [4000000,0.0270139], [4000000,0.0345808], [20000000,0.106954], [20000000,0.113432], [20000000,0.0995448], [20000000,0.101803], [20000000,0.0892393], [16,0.000302853], [16,0.000352654], [16,0.000253695], [16,0.000321454], [16,0.000408721], [1600,0.000291539], [1600,0.000247737], [1600,0.000199986], [1600,0.000151409], [1600,0.000246147], [160000,0.00109487], [160000,0.00086829], [160000,0.00109166], [160000,0.0007622], [160000,0.000904366], [16000000,0.0531622], [16000000,0.0821703], [16000000,0.0579608], [16000000,0.0970074], [16000000,0.076156], [80000000,0.35164], [80000000,0.233693], [80000000,0.36826], [80000000,0.312737], [80000000,0.365286], [24,0.000379587], [24,0.000513089], [24,0.000391193], [24,0.00036039], [24,0.000452403], [2400,0.000427529], [2400,0.000300174], [2400,0.000180323], [2400,0.000301512], [2400,0.000254798], [240000,0.000834355], [240000,0.00122712], [240000,0.000936132], [240000,0.00125675], [240000,0.00108928], [24000000,0.0799388], [24000000,0.104355], [24000000,0.0610003], [24000000,0.102918], [24000000,0.112001], [120000000,0.490345], [120000000,0.339108], [120000000,0.298504], [120000000,0.350729], [120000000,0.521731], [32,0.000415203], [32,0.000426971], [32,0.000335057], [32,0.000524857], [32,0.000490271], [3200,0.000228461], [3200,0.000466977], [3200,0.000371962], [3200,0.000185687], [3200,0.000303334], [320000,0.00143791], [320000,0.00131678], [320000,0.00145501], [320000,0.00121289], [320000,0.00126947], [32000000,0.0823793], [32000000,0.0994834], [32000000,0.0958354], [32000000,0.110345], [32000000,0.102563], [160000000,0.443644], [160000000,0.433892], [160000000,0.443289], [160000000,0.512537], [160000000,0.665968], [54,0.000591004], [54,0.000706332], [54,0.000563412], [54,0.000682444], [54,0.000513743], [5400,0.000489164], [5400,0.000471285], [5400,0.000424697], [5400,0.000410267], [5400,0.000426637], [540000,0.00278073], [540000,0.00261749], [540000,0.00276357], [540000,0.00232736], [540000,0.00261814], [54000000,0.143224], [54000000,0.178732], [54000000,0.166795], [54000000,0.187117], [54000000,0.165245], [270000000,0.57519], [270000000,0.8295], [270000000,0.720886], [270000000,0.600766], [270000000,0.848007]]

def Method_Thread_Avg(Method, thread):
    MethodThreadAvgTime = [0,0,0,0,0]
    MethodThreadAvgRes = [0,0,0,0,0]
    for test in range(0,5):
        MethodThreadAvgRes[0] += Method[thread[oneIter[test]]][0]
        MethodThreadAvgTime[0] += Method[thread[oneIter[test]]][1]
        MethodThreadAvgRes[1] += Method[thread[oneHundredIter[test]]][0]
        MethodThreadAvgTime[1] += Method[thread[oneHundredIter[test]]][1]
        MethodThreadAvgRes[2] += Method[thread[tenThousandIter[test]]][0]
        MethodThreadAvgTime[2] += Method[thread[tenThousandIter[test]]][1]
        MethodThreadAvgRes[3] += Method[thread[oneMillionIter[test]]][0]
        MethodThreadAvgTime[3] += Method[thread[oneMillionIter[test]]][1]
        MethodThreadAvgRes[4] += Method[thread[fiveMilltionIter[test]]][0]
        MethodThreadAvgTime[4] += Method[thread[fiveMilltionIter[test]]][1]
    MethodThreadAvgTime = [x/len(oneIter) for x in MethodThreadAvgTime]
    MethodThreadAvgRes = [x/len(oneIter) for x in MethodThreadAvgRes]
    return (MethodThreadAvgTime, MethodThreadAvgRes)

(Method1OneThreadAvgTime, Method1OneThreadAvgRes) = Method_Thread_Avg(Method1, oneThread)
(Method1FourThreadAvgTime, Method1FourThreadAvgRes) = Method_Thread_Avg(Method1, fourThread)
(Method1SixteenThreadAvgTime, Method1SixteenThreadAvgRes) = Method_Thread_Avg(Method1, sixteenThread)
(Method1TwentyFourThreadAvgTime, Method1TwentyFourThreadAvgRes) = Method_Thread_Avg(Method1, twentyFourThread)
(Method1ThirtyTwoThreadAvgTime, Method1ThirtyTwoThreadAvgRes) = Method_Thread_Avg(Method1, thirtyTwoThread)
(Method1FiftyFourThreadAvgTime, Method1FiftyFourThreadAvgRes) = Method_Thread_Avg(Method1, fiftyFourThread)

(Method2OneThreadAvgTime, Method2OneThreadAvgRes) = Method_Thread_Avg(Method2, oneThread)
(Method2FourThreadAvgTime, Method2FourThreadAvgRes) = Method_Thread_Avg(Method2, fourThread)
(Method2SixteenThreadAvgTime, Method2SixteenThreadAvgRes) = Method_Thread_Avg(Method2, sixteenThread)
(Method2TwentyFourThreadAvgTime, Method2TwentyFourThreadAvgRes) = Method_Thread_Avg(Method2, twentyFourThread)
(Method2ThirtyTwoThreadAvgTime, Method2ThirtyTwoThreadAvgRes) = Method_Thread_Avg(Method2, thirtyTwoThread)
(Method2FiftyFourThreadAvgTime, Method2FiftyFourThreadAvgRes) = Method_Thread_Avg(Method2, fiftyFourThread)

(Method3OneThreadAvgTime, Method3OneThreadAvgRes) = Method_Thread_Avg(Method3, oneThread)
(Method3FourThreadAvgTime, Method3FourThreadAvgRes) = Method_Thread_Avg(Method3, fourThread)
(Method3SixteenThreadAvgTime, Method3SixteenThreadAvgRes) = Method_Thread_Avg(Method3, sixteenThread)
(Method3TwentyFourThreadAvgTime, Method3TwentyFourThreadAvgRes) = Method_Thread_Avg(Method3, twentyFourThread)
(Method3ThirtyTwoThreadAvgTime, Method3ThirtyTwoThreadAvgRes) = Method_Thread_Avg(Method3, thirtyTwoThread)
(Method3FiftyFourThreadAvgTime, Method3FiftyFourThreadAvgRes) = Method_Thread_Avg(Method3, fiftyFourThread)

(Method4OneThreadAvgTime, Method4OneThreadAvgRes) = Method_Thread_Avg(Method4, oneThread)
(Method4FourThreadAvgTime, Method4FourThreadAvgRes) = Method_Thread_Avg(Method4, fourThread)
(Method4SixteenThreadAvgTime, Method4SixteenThreadAvgRes) = Method_Thread_Avg(Method4, sixteenThread)
(Method4TwentyFourThreadAvgTime, Method4TwentyFourThreadAvgRes) = Method_Thread_Avg(Method4, twentyFourThread)
(Method4ThirtyTwoThreadAvgTime, Method4ThirtyTwoThreadAvgRes) = Method_Thread_Avg(Method4, thirtyTwoThread)
(Method4FiftyFourThreadAvgTime, Method4FiftyFourThreadAvgRes) = Method_Thread_Avg(Method4, fiftyFourThread)

(Method5OneThreadAvgTime, Method5OneThreadAvgRes) = Method_Thread_Avg(Method5, oneThread)
(Method5FourThreadAvgTime, Method5FourThreadAvgRes) = Method_Thread_Avg(Method5, fourThread)
(Method5SixteenThreadAvgTime, Method5SixteenThreadAvgRes) = Method_Thread_Avg(Method5, sixteenThread)
(Method5TwentyFourThreadAvgTime, Method5TwentyFourThreadAvgRes) = Method_Thread_Avg(Method5, twentyFourThread)
(Method5ThirtyTwoThreadAvgTime, Method5ThirtyTwoThreadAvgRes) = Method_Thread_Avg(Method5, thirtyTwoThread)
(Method5FiftyFourThreadAvgTime, Method5FiftyFourThreadAvgRes) = Method_Thread_Avg(Method5, fiftyFourThread)

## 1 - 1
pyplot.plot(increments, np.divide(Method1OneThreadAvgRes,Method1OneThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method One, One Thread")
pyplot.savefig("Graphs/MethodOne_1Thread.png")

pyplot.clf()

## 1 - 4
pyplot.plot(increments, np.divide(Method1FourThreadAvgRes,Method1FourThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method One, 4 Threads")
pyplot.savefig("Graphs/MethodOne_4Thread.png")

pyplot.clf()

## 1 - 16
pyplot.plot(increments, np.divide(Method1SixteenThreadAvgRes,Method1SixteenThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method One, 16 Threads")
pyplot.savefig("Graphs/MethodOne_16Thread.png")

pyplot.clf()

## 1 - 24
pyplot.plot(increments, np.divide(Method1ThirtyTwoThreadAvgRes,Method1TwentyFourThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method One, 24 Thread")
pyplot.savefig("Graphs/MethodOne_24Thread.png")

pyplot.clf()

## 1 - 32
pyplot.plot(increments, np.divide(Method1ThirtyTwoThreadAvgRes,Method1ThirtyTwoThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method One, 32 Threads")
pyplot.savefig("Graphs/MethodOne_32Thread.png")

pyplot.clf()

## 1 - 54
pyplot.plot(increments, np.divide(Method1FiftyFourThreadAvgRes,Method1FiftyFourThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method One, 54 Threads")
pyplot.savefig("Graphs/MethodOne_54Thread.png")

pyplot.clf()

################################################################################

## 2 - 1
pyplot.plot(increments, np.divide(Method2OneThreadAvgRes,Method2OneThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method Two, One Thread")
pyplot.savefig("Graphs/MethodTwo_1Thread.png")

pyplot.clf()

## 2 - 4
pyplot.plot(increments, np.divide(Method2FourThreadAvgRes,Method2FourThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method Two, 4 Threads")
pyplot.savefig("Graphs/MethodTwo_4Thread.png")

pyplot.clf()

## 2 - 16
pyplot.plot(increments, np.divide(Method2SixteenThreadAvgRes,Method2SixteenThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method Two, 16 Threads")
pyplot.savefig("Graphs/MethodTwo_16Thread.png")

pyplot.clf()

## 2 - 24
pyplot.plot(increments, np.divide(Method2TwentyFourThreadAvgRes,Method2TwentyFourThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method Two, 24 Thread")
pyplot.savefig("Graphs/MethodTwo_24Thread.png")

pyplot.clf()

## 2 - 32
pyplot.plot(increments, np.divide(Method2ThirtyTwoThreadAvgRes,Method2ThirtyTwoThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method Two, 32 Threads")
pyplot.savefig("Graphs/MethodTwo_32Thread.png")

pyplot.clf()

## 2 - 54
pyplot.plot(increments, np.divide(Method2FiftyFourThreadAvgRes,Method2FiftyFourThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method Two, 54 Threads")
pyplot.savefig("Graphs/MethodTwo_54Thread.png")

pyplot.clf()

################################################################################

## 3 - 1
pyplot.plot(increments, np.divide(Method3OneThreadAvgRes,Method3OneThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method Four, One Thread")
pyplot.savefig("Graphs/MethodFour_1Thread.png")

pyplot.clf()

## 3 - 4
pyplot.plot(increments, np.divide(Method3FourThreadAvgRes,Method3FourThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method Four, 4 Threads")
pyplot.savefig("Graphs/MethodFour_4Thread.png")

pyplot.clf()

## 3 - 16
pyplot.plot(increments, np.divide(Method3SixteenThreadAvgRes,Method3SixteenThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method Four, 16 Threads")
pyplot.savefig("Graphs/MethodFour_16Thread.png")

pyplot.clf()

## 3 - 24
pyplot.plot(increments, np.divide(Method3TwentyFourThreadAvgRes,Method3TwentyFourThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method Four, 24 Thread")
pyplot.savefig("Graphs/MethodFour_24Thread.png")

pyplot.clf()

## 3 - 32
pyplot.plot(increments, np.divide(Method3ThirtyTwoThreadAvgRes,Method3ThirtyTwoThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method Four, 32 Threads")
pyplot.savefig("Graphs/MethodFour_32Thread.png")

pyplot.clf()

## 3 - 54
pyplot.plot(increments, np.divide(Method3FiftyFourThreadAvgRes,Method3FiftyFourThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method Four, 54 Threads")
pyplot.savefig("Graphs/MethodFour_54Thread.png")

pyplot.clf()

################################################################################

## 4 - 1
pyplot.plot(increments, np.divide(Method4OneThreadAvgRes,Method4OneThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method Four, One Thread")
pyplot.savefig("Graphs/MethodFour_1Thread.png")

pyplot.clf()

## 4 - 4
pyplot.plot(increments, np.divide(Method4FourThreadAvgRes,Method4FourThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method Four, 4 Threads")
pyplot.savefig("Graphs/MethodFour_4Thread.png")

pyplot.clf()

## 4 - 16
pyplot.plot(increments, np.divide(Method4SixteenThreadAvgRes,Method4SixteenThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method Four, 16 Threads")
pyplot.savefig("Graphs/MethodFour_16Thread.png")

pyplot.clf()

## 4 - 24
pyplot.plot(increments, np.divide(Method4TwentyFourThreadAvgRes,Method4TwentyFourThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method Four, 24 Thread")
pyplot.savefig("Graphs/MethodFour_24Thread.png")

pyplot.clf()

## 4 - 32
pyplot.plot(increments, np.divide(Method4ThirtyTwoThreadAvgRes,Method4ThirtyTwoThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method Four, 32 Threads")
pyplot.savefig("Graphs/MethodFour_32Thread.png")

pyplot.clf()

## 4 - 54
pyplot.plot(increments, np.divide(Method4FiftyFourThreadAvgRes,Method4FiftyFourThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method Four, 54 Threads")
pyplot.savefig("Graphs/MethodFour_54Thread.png")

pyplot.clf()

################################################################################

## 5 - 1
pyplot.plot(increments, np.divide(Method5OneThreadAvgRes,Method5OneThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method Five, One Thread")
pyplot.savefig("Graphs/MethodFive_1Thread.png")

pyplot.clf()

## 5 - 4
pyplot.plot(increments, np.divide(Method5FourThreadAvgRes,Method5FourThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method Five, 4 Threads")
pyplot.savefig("Graphs/MethodFive_4Thread.png")

pyplot.clf()

## 5 - 16
pyplot.plot(increments, np.divide(Method5SixteenThreadAvgRes,Method5SixteenThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method Five, 16 Threads")
pyplot.savefig("Graphs/MethodFive_16Thread.png")

pyplot.clf()

## 5 - 24
pyplot.plot(increments, np.divide(Method5TwentyFourThreadAvgRes,Method3TwentyFourThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method Five, 24 Thread")
pyplot.savefig("Graphs/MethodFive_24Thread.png")

pyplot.clf()

## 5 - 32
pyplot.plot(increments, np.divide(Method5ThirtyTwoThreadAvgRes,Method5ThirtyTwoThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method Five, 32 Threads")
pyplot.savefig("Graphs/MethodFive_32Thread.png")

pyplot.clf()

## 5 - 54
pyplot.plot(increments, np.divide(Method5FiftyFourThreadAvgRes,Method5FiftyFourThreadAvgTime), 'ro')
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Increments per Thread")
pyplot.title("Method Five, 54 Threads")
pyplot.savefig("Graphs/MethodFive_54Thread.png")

pyplot.clf()


oneMillionIterOverMethods = [np.divide(Method1ThirtyTwoThreadAvgRes[4], Method1ThirtyTwoThreadAvgTime[4]), np.divide(Method2ThirtyTwoThreadAvgRes[4], Method2ThirtyTwoThreadAvgTime[4]), np.divide(Method3ThirtyTwoThreadAvgRes[4], Method3ThirtyTwoThreadAvgTime[4]), np.divide(Method4ThirtyTwoThreadAvgRes[4], Method4ThirtyTwoThreadAvgTime[4]), np.divide(Method5ThirtyTwoThreadAvgRes[4], Method5ThirtyTwoThreadAvgTime[4])]
print(oneMillionIterOverMethods)
print([Method1ThirtyTwoThreadAvgRes[4], Method2ThirtyTwoThreadAvgRes[4], Method3ThirtyTwoThreadAvgRes[4], Method4ThirtyTwoThreadAvgRes[4], Method5ThirtyTwoThreadAvgRes[4]])
print([Method1ThirtyTwoThreadAvgTime[4], Method2ThirtyTwoThreadAvgTime[4], Method3ThirtyTwoThreadAvgTime[4], Method4ThirtyTwoThreadAvgTime[4], Method5ThirtyTwoThreadAvgTime[4]])
## one Million Iterations
pyplot.plot([1,2,3,4,5], oneMillionIterOverMethods, 'ro');
pyplot.ylabel("Increments/sec")
pyplot.xlabel("Method")
pyplot.title("1,000,000 Increments per thread, 16 threads")
pyplot.savefig("Graphs/MethodCmp.png")

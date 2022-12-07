#include "lab_m1/Tema2/Tema2.h"
#include "lab_m1/Tema2/object3D.h"

#include <iostream>
#include <string>
#include <vector>

#include "lab_m1/Tema2/transform3D.h"

using namespace std;
using namespace m1;

Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
	camera = new implemented::HwCamera();
	camera->Set(glm::vec3(0, 1.2f, 6.0f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	camera->RotateThirdPerson_OY(angle);
	camera2 = new implemented::HwCamera();

	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("plane");
		mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader* shader = new Shader("LabShader");
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
		shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	//Se genereaza punctele initiale ale pistei
	{
		GetPointsArray();
		Mesh* mesh = object3D::CreateTrack(in, ext, glm::vec3(0, 0, 0));
		AddMeshToList(mesh);
	}
	// Sets the resolution of the small viewport
	glm::ivec2 resolution = window->GetResolution();
	miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);

	//setari camera
	fov = 60.0f;
	left = -15.0f;
	right = 15.0f;
	up = 15.0f;
	down = -15.0f;

	//proiectie camera principala
	projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);
	//proiectie minimap
	projectionMatrixOrtho = glm::ortho(left, right, down, up, 0.01f, 200.f);

}


void Tema2::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0.4f, 0, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

//se initializeaza vectorul de puncte
void Tema2::GetPointsArray() {
	initialPoints = {2.75f * glm::vec3(-19.926427448670744, 0, 0.2307757376257994),
	2.75f * glm::vec3(-19.63517575870129, 0, 0.6385281035830366),
	2.75f * glm::vec3(-19.30509051006924, 0, 1.0851140282028677),
	2.75f * glm::vec3(-18.877921364780704, 0, 1.6482006288104811),
	2.75f * glm::vec3(-18.470168998823468, 0, 2.114203332761609),
	2.75f * glm::vec3(-18.023583074203636, 0, 2.5413724780501434),
	2.75f * glm::vec3(-17.576997149583804, 0, 2.9685416233386777),
	2.75f * glm::vec3(-17.052744107638784, 0, 3.415127547958509),
	2.75f * glm::vec3(-16.54790784502506, 0, 3.822879913915746),
	2.75f * glm::vec3(-16.08190514107393, 0, 4.172381941879093),
	2.75f * glm::vec3(-15.712986333779286, 0, 4.4442168525172505),
	2.75f * glm::vec3(-15.188733291834266, 0, 4.735468542486705),
	2.75f * glm::vec3(-14.664480249889246, 0, 4.987886673793566),
	2.75f * glm::vec3(-14.198477545938117, 0, 5.259721584431725),
	2.75f * glm::vec3(-13.654807724661799, 0, 5.531556495069883),
	2.75f * glm::vec3(-13.149971462048077, 0, 5.725724288382853),
	2.75f * glm::vec3(-12.586884861440462, 0, 5.919892081695823),
	2.75f * glm::vec3(-12.043215040164144, 0, 6.152893433671387),
	2.75f * glm::vec3(-11.460711660225233, 0, 6.269394109659169),
	2.75f * glm::vec3(-10.994708956274104, 0, 6.347061226984358),
	2.75f * glm::vec3(-10.412205576335193, 0, 6.347061226984358),
	2.75f * glm::vec3(-9.946202872384063, 0, 6.327644447653061),
	2.75f * glm::vec3(-9.421949830439043, 0, 6.347061226984358),
	2.75f * glm::vec3(-8.936530347156618, 0, 6.327644447653061),
	2.75f * glm::vec3(-8.509361201868083, 0, 6.308227668321763),
	2.75f * glm::vec3(-8.023941718585657, 0, 6.269394109659169),
	2.75f * glm::vec3(-7.655022911291013, 0, 6.191726992333981),
	2.75f * glm::vec3(-7.150186648677289, 0, 6.055809537014902),
	2.75f * glm::vec3(-6.723017503388755, 0, 5.842224964370635),
	2.75f * glm::vec3(-6.373515475425408, 0, 5.609223612395071),
	2.75f * glm::vec3(-5.985179888799467, 0, 5.317971922425616),
	2.75f * glm::vec3(-5.674511419498715, 0, 4.987886673793566),
	2.75f * glm::vec3(-5.383259729529259, 0, 4.677218204492815),
	2.75f * glm::vec3(-5.1114248188911, 0, 4.327716176529468),
	2.75f * glm::vec3(-4.8201731289216445, 0, 3.8811302519096373),
	2.75f * glm::vec3(-4.58717177694608, 0, 3.512211444614994),
	2.75f * glm::vec3(-4.334753645639219, 0, 3.123875857989054),
	2.75f * glm::vec3(-4.043501955669763, 0, 2.7743738300257075),
	2.75f * glm::vec3(-3.713416707037713, 0, 2.5025389193875496),
	2.75f * glm::vec3(-3.4027482377369602, 0, 2.2889543467432825),
	2.75f * glm::vec3(-3.1309133270988014, 0, 2.114203332761609),
	2.75f * glm::vec3(-2.7619945198041576, 0, 1.958869098111233),
	2.75f * glm::vec3(-2.315408595184326, 0, 1.8035348634608572),
	2.75f * glm::vec3(-1.8299891119018996, 0, 1.6482006288104811),
	2.75f * glm::vec3(-1.3834031872820676, 0, 1.492866394160105),
	2.75f * glm::vec3(-1.014484379987424, 0, 1.376365718172323),
	2.75f * glm::vec3(-0.5873152346988888, 0, 1.337532159509729),
	2.75f * glm::vec3(-0.21062567080651567, 0, 1.3090916883653818),
	2.75f * glm::vec3(0.23128223604554804, 0, 1.3207208438088571),
	2.75f * glm::vec3(0.7439187242808457, 0, 1.4218636394716044),
	2.75f * glm::vec3(1.289148062501529, 0, 1.6399553747598765),
	2.75f * glm::vec3(1.6781929214667077, 0, 1.859537726214419),
	2.75f * glm::vec3(2.1126218438838973, 0, 2.1657545979326738),
	2.75f * glm::vec3(2.4133632440173574, 0, 2.42890332304945),
	2.75f * glm::vec3(2.7516973191674996, 0, 2.6732557106578847),
	2.75f * glm::vec3(3, 0, 3),
	2.75f * glm::vec3(3.315587444417737, 0, 3.3123311859414835),
	2.75f * glm::vec3(3.578736169534514, 0, 3.650665261091624),
	2.75f * glm::vec3(3.8418848946512916, 0, 3.8950176487000587),
	2.75f * glm::vec3(4.123829957276411, 0, 4.158166373816835),
	2.75f * glm::vec3(4.42457135740987, 0, 4.440111436441952),
	2.75f * glm::vec3(4.744109095051671, 0, 4.759649174083751),
	2.75f * glm::vec3(5.157628520235178, 0, 5.0979832492338915),
	2.75f * glm::vec3(5.514758932893662, 0, 5.361131974350668),
	2.75f * glm::vec3(5.834296670535464, 0, 5.661873374484126),
	2.75f * glm::vec3(6.247816095718971, 0, 5.943818437109243),
	2.75f * glm::vec3(6.680131858410819, 0, 6.244559837242702),
	2.75f * glm::vec3(7.150040296119351, 0, 6.54530123737616),
	2.75f * glm::vec3(7.601152396319541, 0, 6.789653624984594),
	2.75f * glm::vec3(8, 0, 7),
	2.75f * glm::vec3(8.409394909178214, 0, 7.221969387676441),
	2.75f * glm::vec3(8.80411799685338, 0, 7.391136425251511),
	2.75f * glm::vec3(9.217637422036887, 0, 7.560303462826581),
	2.75f * glm::vec3(9.70634219725376, 0, 7.71067416289331),
	2.75f * glm::vec3(10.232639647487314, 0, 7.842248525451698),
	2.75f * glm::vec3(10.683751747687504, 0, 7.917433875485063),
	2.75f * glm::vec3(11.247641872937741, 0, 7.955026550501745),
	2.75f * glm::vec3(11.755142985662955, 0, 7.955026550501745),
	2.75f * glm::vec3(12.243847760879827, 0, 7.955026550501745),
	2.75f * glm::vec3(12.60097817353831, 0, 7.861044862960039),
	2.75f * glm::vec3(13.014497598721817, 0, 7.673081487876628),
	2.75f * glm::vec3(13.446813361413666, 0, 7.428729100268193),
	2.75f * glm::vec3(13.80394377407215, 0, 7.165580375151417),
	2.75f * glm::vec3(14.198666861747316, 0, 6.789653624984594),
	2.75f * glm::vec3(14.518204599389117, 0, 6.376134199801089),
	2.75f * glm::vec3(14.856538674539259, 0, 5.962614774617584),
	2.75f * glm::vec3(15.100891062147696, 0, 5.643077036975785),
	2.75f * glm::vec3(15.439225137297838, 0, 5.267150286808962),
	2.75f * glm::vec3(15.796355549956322, 0, 4.778445511592093),
	2.75f * glm::vec3(16.115893287598123, 0, 4.402518761425269),
	2.75f * glm::vec3(16.454227362748266, 0, 4.007795673750105),
	2.75f * glm::vec3(16.754968762881724, 0, 3.6318689235832826),
	2.75f * glm::vec3(17.13089551304855, 0, 3.1807568233830956),
	2.75f * glm::vec3(17.469229588198694, 0, 2.8048300732162725),
	2.75f * glm::vec3(17.75117465082381, 0, 2.4852923355744734),
	2.75f * glm::vec3(18.014323375940588, 0, 2.184550935441015),
	2.75f * glm::vec3(18.33386111358239, 0, 1.8086241852741922),
	2.75f * glm::vec3(18.61580617620751, 0, 1.4702901101240518),
	2.75f * glm::vec3(18.897751238832626, 0, 1.1507523724822524),
	2.75f * glm::vec3(19.179696301457746, 0, 0.8500109723487941),
	2.75f * glm::vec3(19.499234039099548, 0, 0.49288055969031247),
	2.75f * glm::vec3(19.74358642670798, 0, 0.17334282204851306),
	2.75f * glm::vec3(19.843725460657417, 0, -0.11541164116513802),
	2.75f * glm::vec3(19.843725460657417, 0, -0.3719727059376543),
	2.75f * glm::vec3(19.744718742876287, 0, -0.6865617934180918),
	2.75f * glm::vec3(19.59842227761584, 0, -1.0376733100431677),
	2.75f * glm::vec3(19.437496165829344, 0, -1.3448958870901093),
	2.75f * glm::vec3(19.232681114464718, 0, -1.6521184641370508),
	2.75f * glm::vec3(19, 0, -2),
	2.75f * glm::vec3(18.73527313257919, 0, -2.310452557809068),
	2.75f * glm::vec3(18.442680202058295, 0, -2.58841584180392),
	2.75f * glm::vec3(18.223235504167622, 0, -2.778601246642503),
	2.75f * glm::vec3(17.84286469449046, 0, -3.100453470215489),
	2.75f * glm::vec3(17.49175317786538, 0, -3.3198981681061617),
	2.75f * glm::vec3(17.111382368188213, 0, -3.5832318055749686),
	2.75f * glm::vec3(16.789530144615227, 0, -3.744157917361462),
	2.75f * glm::vec3(16.36527039535993, 0, -3.9489729687260895),
	2.75f * glm::vec3(16.058047818312986, 0, -4.153788020090717),
	2.75f * glm::vec3(15.692306655161865, 0, -4.31471413187721),
	2.75f * glm::vec3(15.326565492010742, 0, -4.446380950611614),
	2.75f * glm::vec3(14.902305742755441, 0, -4.592677415872062),
	2.75f * glm::vec3(14.565823872656411, 0, -4.695084941554376),
	2.75f * glm::vec3(14.1708234164532, 0, -4.753603527658555),
	2.75f * glm::vec3(13.717304374145808, 0, -4.636566355450197),
	2.75f * glm::vec3(13.395452150572822, 0, -4.446380950611614),
	2.75f * glm::vec3(13.044340633947746, 0, -4.212306606194897),
	2.75f * glm::vec3(12.69322911732267, 0, -3.8904543826219102),
	2.75f * glm::vec3(12.283599014593413, 0, -3.5978614521010135),
	2.75f * glm::vec3(11.932487497968337, 0, -3.3198981681061617),
	2.75f * glm::vec3(11.596005627869305, 0, -3.01267559105922),
	2.75f * glm::vec3(11.27415340429632, 0, -2.705453014012279),
	2.75f * glm::vec3(10.937671534197287, 0, -2.4713786695955613),
	2.75f * glm::vec3(10.513411784941987, 0, -2.310452557809068),
	2.75f * glm::vec3(10.04526309610855, 0, -2.1787857390746646),
	2.75f * glm::vec3(9.62100334685325, 0, -2.149526446022575),
	2.75f * glm::vec3(9.16748430454586, 0, -2.13489679949653),
	2.75f * glm::vec3(8.816372787920784, 0, -2.2080450321267544),
	2.75f * glm::vec3(8.333594452561304, 0, -2.3543414973872028),
	2.75f * glm::vec3(7.997112582462272, 0, -2.5298972556997406),
	2.75f * glm::vec3(7.558223186680927, 0, -2.8371198327466822),
	2.75f * glm::vec3(7.23637096310794, 0, -3.1443424097936234),
	2.75f * glm::vec3(6.885259446482864, 0, -3.5100835729447444),
	2.75f * glm::vec3(6.607296162488011, 0, -3.817306149991686),
	2.75f * glm::vec3(6.226925352810845, 0, -4.168417666616762),
	2.75f * glm::vec3(5.831924896607634, 0, -4.475640243663704),
	2.75f * glm::vec3(5.422294793878378, 0, -4.812122113762735),
	2.75f * glm::vec3(4.822479286310539, 0, -5.22175221649199),
	2.75f * glm::vec3(4.398219537055239, 0, -5.499715500486842),
	2.75f * glm::vec3(3.944700494747848, 0, -5.6021230261691555),
	2.75f * glm::vec3(3.3448849871800093, 0, -5.558234086591021),
	2.75f * glm::vec3(2.671921246981946, 0, -5.426567267856618),
	2.75f * glm::vec3(2.203772558148511, 0, -5.28027080259617),
	2.75f * glm::vec3(1.6478459901588065, 0, -5.1047150442836315),
	2.75f * glm::vec3(1.1796973013253713, 0, -4.8706406998669145),
	2.75f * glm::vec3(0.7408079055440258, 0, -4.680455295028331),
	2.75f * glm::vec3(0.3604370958668596, 0, -4.563418122819972),
	2.75f * glm::vec3(0.08247381187200743, 0, -4.461010597137658),
	2.75f * glm::vec3(-0.268637704753069, 0, -4.37323271798139),
	2.75f * glm::vec3(-0.5758602818000109, 0, -4.461010597137658),
	2.75f * glm::vec3(-0.8836273221663669, 0, -4.6425600341882385),
	2.75f * glm::vec3(-1.231754539676872, 0, -4.91332564780752),
	2.75f * glm::vec3(-1.6572433610786008, 0, -5.164750860453995),
	2.75f * glm::vec3(-2.140753385398747, 0, -5.454856875046082),
	2.75f * glm::vec3(-2.5662422068004758, 0, -5.706282087692557),
	2.75f * glm::vec3(-3, 0, -6),
	2.75f * glm::vec3(-3.513921854467962, 0, -6.131770909094285),
	2.75f * glm::vec3(-3.9780914778153025, 0, -6.189792112012702),
	2.75f * glm::vec3(-4.59698430894509, 0, -6.170451711039896),
	2.75f * glm::vec3(-5, 0, -6),
	2.75f * glm::vec3(-5.467302352721353, 0, -5.532218478937305),
	2.75f * glm::vec3(-5.738067966340634, 0, -5.145410459481189),
	2.75f * glm::vec3(-6.105535584823945, 0, -4.700581237106656),
	2.75f * glm::vec3(-6.376301198443228, 0, -4.333113618623346),
	2.75f * glm::vec3(-6.763109217899344, 0, -3.8109227923575895),
	2.75f * glm::vec3(-7.091896034437044, 0, -3.520816777765502),
	2.75f * glm::vec3(-7.614086860702802, 0, -3.385433970955862),
	2.75f * glm::vec3(-8.078256484050142, 0, -3.3660935699830556),
	2.75f * glm::vec3(-8.619787711288705, 0, -3.4241147729014734),
	2.75f * glm::vec3(-8.987255329772017, 0, -3.636859183602337),
	2.75f * glm::vec3(-9.354722948255327, 0, -4.062348005004065),
	2.75f * glm::vec3(-9.62548856187461, 0, -4.429815623487375),
	2.75f * glm::vec3(-9.915594576466697, 0, -4.700581237106656),
	2.75f * glm::vec3(-10.32174299689562, 0, -5.145410459481189),
	2.75f * glm::vec3(-10.708551016351738, 0, -5.454856875046082),
	2.75f * glm::vec3(-11.172720639699078, 0, -5.78364369158378),
	2.75f * glm::vec3(-11.752932668883252, 0, -5.977047701311839),
	2.75f * glm::vec3(-12.37182550001304, 0, -6.209132512985508),
	2.75f * glm::vec3(-12.93269712822441, 0, -6.228472913958314),
	2.75f * glm::vec3(-13.62895156324542, 0, -6.093090107148673),
	2.75f * glm::vec3(-14.035099983674343, 0, -5.802984092556587),
	2.75f * glm::vec3(-14.673333215776935, 0, -5.300133667263635),
	2.75f * glm::vec3(-15.156843240097082, 0, -4.855304444889103),
	2.75f * glm::vec3(-15.67903406636284, 0, -4.371794420568958),
	2.75f * glm::vec3(-16.2012248926286, 0, -4.062348005004065),
	2.75f * glm::vec3(-16.684734916948745, 0, -3.9076247972216183),
	2.75f * glm::vec3(-17.26494694613292, 0, -3.7529015894391717),
	2.75f * glm::vec3(-17.941860980181122, 0, -3.6175187826295314),
	2.75f * glm::vec3(-18.502732608392492, 0, -3.462795574847085),
	2.75f * glm::vec3(-18.98624263271264, 0, -3.192029961227804),
	2.75f * glm::vec3(-19.45041225605998, 0, -2.8439027437172992),
	2.75f * glm::vec3(-19.759858671624873, 0, -2.4377543232883774),
	2.75f * glm::vec3(-20, 0, -2),
	2.75f * glm::vec3(-20.166007092053796, 0, -1.509415076593699),
	2.75f * glm::vec3(-20.282049497890632, 0, -1.1226070571375828),
	2.75f * glm::vec3(-20.301389898863437, 0, -0.6777778347630494),
	2.75f * glm::vec3(-20.224028294972214, 0, -0.21360821141571015) };
	//coordonatele initiale pt copaci
	trees.insert(trees.begin(), initialPoints.begin(), initialPoints.end());
	//coordonatele pentru prima masina
	car1.insert(car1.begin(), initialPoints.begin(), initialPoints.end());
	//append la primele 2 puncte pentru a imbina pista in zona de inceput
	initialPoints.push_back(initialPoints[0]);
	initialPoints.push_back(initialPoints[1]);
	//pt a genera mai multe triunghiuri => pista mai smooth
	int len = initialPoints.size();
	for (int i = 1; i < len; i++) {
		initialPoints.push_back(initialPoints[len - i]);
	}

	//coordonate interne externe pt pista
	for (int i = 0, j = 1; i < initialPoints.size() && j < initialPoints.size(); i++, j++) {
		glm::vec3 D = initialPoints[j] - initialPoints[i];
		glm::vec3 P = glm::normalize(glm::cross(D, glm::vec3(0, 1, 0)));
		in.push_back((initialPoints[i] - 2.0f * P));
		ext.push_back((initialPoints[i] + 2.0f * P));
	}

	//generarea coordonatelor celorlaltor 2 masini
	for (int i = 0, j = 1; i < car1.size() && j < car1.size(); i++, j++) {
		glm::vec3 D = car1[j] - car1[i];
		glm::vec3 P = glm::normalize(glm::cross(D, glm::vec3(0, 1, 0)));
		car2.push_back((car1[i] - P));
		car3.push_back((car1[i] + P));
	}
}

//functie ce calculeaza distanta dintre 2 puncte
float Tema2::distance(glm::vec3 p1, glm::vec3 p2) {
	return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.z - p1.z) * (p2.z - p1.z));
}

//functie care calculeaza aria unui triunghi
float Tema2::TriangleArea(glm::vec3 A, glm::vec3 B, glm::vec3 C) {
	glm::vec3 AB = glm::vec3(B.x - A.x, 0, B.z - A.z);
	glm::vec3 AC = glm::vec3(C.x - A.x, 0, C.z - A.z);
	float cross_prod = AB.x * AC.z - AB.z * AC.x;
	float triangleArea = abs(cross_prod / 2);
	return triangleArea;
}

//randeaza scena principala
void Tema2::RenderScene(float deltaTimeSeconds) {
	{
		if (car1Pos == car1.size() - 1) {
			car1Pos = 0;
		}
		if (car2Pos == car2.size() - 1) {
			car2Pos = 0;
		}
		if (car3Pos == car3.size() - 1) {
			car3Pos = 0;
		}
		//pemtru fiecare masina se calculeaza vectorul forward
		glm::vec3 p1 = car1[car1Pos], p2 = car1[car1Pos + 1];
		glm::vec3 p21 = car2[car2Pos], p22 = car2[car2Pos + 1];
		glm::vec3 p31 = car3[car3Pos], p32 = car3[car3Pos + 1];
		glm::vec3 forward1 = glm::normalize(p2 - p1);
		glm::vec3 forward2 = glm::normalize(p22 - p21);
		glm::vec3 forward3 = glm::normalize(p32 - p31);
		//se calculeaza pozitiile (masinile au viteze diferite de deplasare)
		firstCarPos += glm::normalize(p2 - p1) * deltaTimeSeconds * 3.0f;
		secCarPos += glm::normalize(p22 - p21) * deltaTimeSeconds * 4.0f;
		thCarPos += glm::normalize(p32 - p31) * deltaTimeSeconds * 5.0f;
		//trecerea in urmatorul segment al pistei
		if (distance(p2, p1) <= distance(firstCarPos, p1)) {
			firstCarPos = p2;
			car1Pos++;
		}
		if (distance(p22, p21) <= distance(secCarPos, p21)) {
			secCarPos = p22;
			car2Pos++;
		}
		if (distance(p32, p31) <= distance(thCarPos, p31)) {
			thCarPos = p32;
			car3Pos++;
		}

		//calcularea unghiului de rotatie folosind vectorul forward
		glm::mat4 modelMatrix1 = glm::mat4(1);
		modelMatrix1 *= transform3D::Translate(firstCarPos.x, 0.5f, firstCarPos.z);
		modelMatrix1 *= transform3D::RotateOY(atan2(forward1.x, forward1.z));
		modelMatrix1 *= transform3D::Scale(0.25f, 0.13f, 0.7f);
		RenderSimpleMesh(meshes["box"], shaders["LabShader"], true, modelMatrix1, glm::vec3(0, 0, 1));

		glm::mat4 modelMatrix2 = glm::mat4(1);
		modelMatrix2 *= transform3D::Translate(secCarPos.x, 0.5f, secCarPos.z);
		modelMatrix2 *= transform3D::RotateOY(atan2(forward2.x, forward2.z));
		modelMatrix2 *= transform3D::Scale(0.25f, 0.13f, 0.7f);
		RenderSimpleMesh(meshes["box"], shaders["LabShader"], true, modelMatrix2, glm::vec3(1, 0, 1));

		glm::mat4 modelMatrix3 = glm::mat4(1);
		modelMatrix3 *= transform3D::Translate(thCarPos.x, 0.5f, thCarPos.z);
		modelMatrix3 *= transform3D::RotateOY(atan2(forward3.x, forward3.z));
		modelMatrix3 *= transform3D::Scale(0.25f, 0.13f, 0.7f);
		RenderSimpleMesh(meshes["box"], shaders["LabShader"], true, modelMatrix3, glm::vec3(0, 1, 1));
	}

	{
		glm::vec3 pos = camera->GetTargetPosition();
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= transform3D::Translate(pos.x, pos.y - 0.675f, pos.z);
		modelMatrix *= transform3D::RotateOY(-angle);
		modelMatrix *= transform3D::Scale(0.25f, 0.13f, 0.7f);
		RenderSimpleMesh(meshes["box"], shaders["LabShader"], true, modelMatrix, glm::vec3(1, 0, 0));
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= transform3D::Translate(0, 0.4f, 0);
		RenderSimpleMesh(meshes["track"], shaders["LabShader"], true, modelMatrix, glm::vec3(0, 0, 0));
	}

	{
	    glm::mat4 modelMatrix = glm::mat4(1);
	    modelMatrix *= transform3D::Translate(0, 0.35f, 0);
	    modelMatrix *= transform3D::Scale(4.0f, 0, 4.0f);
	    RenderSimpleMesh(meshes["plane"], shaders["LabShader"], true, modelMatrix, glm::vec3(0, 1, 0));
	}

	//randare copaci
	{
		for (int k = 1; k < 3; k++) {
			for (int i = 0, j = 1; i < trees.size() && j < trees.size(); i++, j++) {
				if (i % 4 == 0 || i % 5 == 0) {
					continue;
				}
				if (i % 3 == 0 && k == 2) {
					continue;
				}
				glm::vec3 D = trees[j] - trees[i];
				glm::vec3 P = glm::normalize(glm::cross(D, glm::vec3(0, 1, 0)));
				float spacing = 2.0f + 2 * k;
				glm::vec3 inTree = trees[i] - spacing * P;
				glm::vec3 outTree = trees[i] + spacing * P;
				glm::mat4 modelMatrix1 = glm::mat4(1);
				modelMatrix1 *= transform3D::Translate(inTree.x, 0.5f, inTree.z);
				modelMatrix1 *= transform3D::Scale(0.25f, 0.75f, 0.25f);
				RenderSimpleMesh(meshes["box"], shaders["LabShader"], true, modelMatrix1, glm::vec3(0.588f, 0.29f, 0));
				glm::mat4 modelMatrix2 = glm::mat4(1);
				modelMatrix2 *= transform3D::Translate(inTree.x, 1.1f, inTree.z);
				modelMatrix2 *= transform3D::Scale(1.0f, 1.0f, 1.0f);
				RenderSimpleMesh(meshes["box"], shaders["LabShader"], true, modelMatrix2, glm::vec3(0, 0.5f, 0));
				glm::mat4 modelMatrix3 = glm::mat4(1);
				modelMatrix3 *= transform3D::Translate(outTree.x, 0.5f, outTree.z);
				modelMatrix3 *= transform3D::Scale(0.25f, 0.75f, 0.25f);
				RenderSimpleMesh(meshes["box"], shaders["LabShader"], true, modelMatrix3, glm::vec3(0.588f, 0.29f, 0));
				glm::mat4 modelMatrix4 = glm::mat4(1);
				modelMatrix4 *= transform3D::Translate(outTree.x, 1.1f, outTree.z);
				modelMatrix4 *= transform3D::Scale(1.0f, 1.0f, 1.0f);
				RenderSimpleMesh(meshes["box"], shaders["LabShader"], true, modelMatrix4, glm::vec3(0, 0.5f, 0));
			}
		}
	}

	//randare linie discontinua
	{
		for (int i = 0; i < car1.size(); i += 2) {
			glm::vec3 forward;
			if (i == car1.size() - 1) {
				forward = glm::normalize(car1[0] - car1[i]);
			}
			else {
				forward = glm::normalize(car1[i + 1] - car1[i]);
			}
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= transform3D::Translate(car1[i].x, 0.38f, car1[i].z);
			modelMatrix *= transform3D::RotateOY(atan2(forward.x, forward.z));
			modelMatrix *= transform3D::Scale(0.25f, 0.001f, 1.0f);
			RenderSimpleMesh(meshes["box"], shaders["LabShader"], true, modelMatrix, glm::vec3(1, 1, 1));
		}
	}
}

//randeaza scena minimap (acceasi ca mai sus doar ca se fol proiectia ortografica)
void Tema2::RenderSceneOrtho(float deltaTimeSeconds) {
	{
		if (car1Pos == car1.size() - 1) {
			car1Pos = 0;
		}
		if (car2Pos == car2.size() - 1) {
			car2Pos = 0;
		}
		if (car3Pos == car3.size() - 1) {
			car3Pos = 0;
		}
		glm::vec3 p1 = car1[car1Pos], p2 = car1[car1Pos + 1];
		glm::vec3 p21 = car2[car2Pos], p22 = car2[car2Pos + 1];
		glm::vec3 p31 = car3[car3Pos], p32 = car3[car3Pos + 1];
		glm::vec3 forward1 = glm::normalize(p2 - p1);
		glm::vec3 forward2 = glm::normalize(p22 - p21);
		glm::vec3 forward3 = glm::normalize(p32 - p31);
		firstCarPos += glm::normalize(p2 - p1) * deltaTimeSeconds * 3.0f;
		secCarPos += glm::normalize(p22 - p21) * deltaTimeSeconds * 4.0f;
		thCarPos += glm::normalize(p32 - p31) * deltaTimeSeconds * 5.0f;
		if (distance(p2, p1) <= distance(firstCarPos, p1)) {
			firstCarPos = p2;
			car1Pos++;
		}
		if (distance(p22, p21) <= distance(secCarPos, p21)) {
			secCarPos = p22;
			car2Pos++;
		}
		if (distance(p32, p31) <= distance(thCarPos, p31)) {
			thCarPos = p32;
			car3Pos++;
		}

		glm::mat4 modelMatrix1 = glm::mat4(1);
		modelMatrix1 *= transform3D::Translate(firstCarPos.x, 0.5f, firstCarPos.z);
		modelMatrix1 *= transform3D::RotateOY(atan2(forward1.x, forward1.z));
		modelMatrix1 *= transform3D::Scale(0.6f, 0.6f, 1.5f);
		RenderSimpleMesh(meshes["box"], shaders["LabShader"], false, modelMatrix1, glm::vec3(0, 0, 1));

		glm::mat4 modelMatrix2 = glm::mat4(1);
		modelMatrix2 *= transform3D::Translate(secCarPos.x, 0.5f, secCarPos.z);
		modelMatrix2 *= transform3D::RotateOY(atan2(forward2.x, forward2.z));
		modelMatrix2 *= transform3D::Scale(0.6f, 0.6f, 1.5f);
		RenderSimpleMesh(meshes["box"], shaders["LabShader"], false, modelMatrix2, glm::vec3(1, 0, 1));

		glm::mat4 modelMatrix3 = glm::mat4(1);
		modelMatrix3 *= transform3D::Translate(thCarPos.x, 0.5f, thCarPos.z);
		modelMatrix3 *= transform3D::RotateOY(atan2(forward3.x, forward3.z));
		modelMatrix3 *= transform3D::Scale(0.6f, 0.6f, 1.5f);
		RenderSimpleMesh(meshes["box"], shaders["LabShader"], false, modelMatrix3, glm::vec3(0, 1, 1));
	}

	{
		glm::vec3 pos = camera->GetTargetPosition();
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= transform3D::Translate(pos.x, pos.y - 0.675f, pos.z);
		modelMatrix *= transform3D::RotateOY(-angle);
		modelMatrix *= transform3D::Scale(0.6f, 0.6f, 1.5f);
		RenderSimpleMesh(meshes["box"], shaders["LabShader"], false, modelMatrix, glm::vec3(1, 0, 0));
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= transform3D::Translate(0, 0.4f, 0);
		RenderSimpleMesh(meshes["track"], shaders["LabShader"], false, modelMatrix, glm::vec3(0, 0, 0));
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix *= transform3D::Translate(0, 0.2f, 0);
		modelMatrix *= transform3D::Scale(100.0f, 0, 100.0f);
		RenderSimpleMesh(meshes["plane"], shaders["LabShader"], false, modelMatrix, glm::vec3(0, 1, 0));
	}

	{
		for (int k = 1; k < 3; k++) {
			for (int i = 0, j = 1; i < trees.size() && j < trees.size(); i++, j++) {
				if (i % 4 == 0 || i % 5 == 0) {
					continue;
				}
				if (i % 3 == 0 && k == 2) {
					continue;
				}
				glm::vec3 D = trees[j] - trees[i];
				glm::vec3 P = glm::normalize(glm::cross(D, glm::vec3(0, 1, 0)));
				float spacing = 2.0f + 2 * k;
				glm::vec3 inTree = trees[i] - spacing * P;
				glm::vec3 outTree = trees[i] + spacing * P;
				glm::mat4 modelMatrix1 = glm::mat4(1);
				modelMatrix1 *= transform3D::Translate(inTree.x, 0.5f, inTree.z);
				modelMatrix1 *= transform3D::Scale(0.25f, 0.75f, 0.25f);
				RenderSimpleMesh(meshes["box"], shaders["LabShader"], false, modelMatrix1, glm::vec3(0.588f, 0.29f, 0));
				glm::mat4 modelMatrix2 = glm::mat4(1);
				modelMatrix2 *= transform3D::Translate(inTree.x, 1.1f, inTree.z);
				modelMatrix2 *= transform3D::Scale(1.0f, 1.0f, 1.0f);
				RenderSimpleMesh(meshes["box"], shaders["LabShader"], false, modelMatrix2, glm::vec3(0, 0.5f, 0));
				glm::mat4 modelMatrix3 = glm::mat4(1);
				modelMatrix3 *= transform3D::Translate(outTree.x, 0.5f, outTree.z);
				modelMatrix3 *= transform3D::Scale(0.25f, 0.75f, 0.25f);
				RenderSimpleMesh(meshes["box"], shaders["LabShader"], false, modelMatrix3, glm::vec3(0.588f, 0.29f, 0));
				glm::mat4 modelMatrix4 = glm::mat4(1);
				modelMatrix4 *= transform3D::Translate(outTree.x, 1.1f, outTree.z);
				modelMatrix4 *= transform3D::Scale(1.0f, 1.0f, 1.0f);
				RenderSimpleMesh(meshes["box"], shaders["LabShader"], false, modelMatrix4, glm::vec3(0, 0.5f, 0));
			}
		}
	}

	//randarea liniei discontinue pe minimap
	{
		for (int i = 0; i < car1.size(); i++) {
			glm::vec3 forward;
			if (i == car1.size() - 1) {
				forward = glm::normalize(car1[0] - car1[i]);
			}
			else {
				forward = glm::normalize(car1[i + 1] - car1[i]);
			}
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix *= transform3D::Translate(car1[i].x, 0.38f, car1[i].z);
			modelMatrix *= transform3D::RotateOY(atan2(forward.x, forward.z));
			modelMatrix *= transform3D::Scale(0.25f, 0.001f, 0.45f);
			RenderSimpleMesh(meshes["box"], shaders["LabShader"], false, modelMatrix, glm::vec3(1, 1, 1));
		}
	}
}

void Tema2::Update(float deltaTimeSeconds)
{
	glLineWidth(3);
	glPointSize(5);

	// se randeaza cele 2 scene (principala / minimap)
	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);

	RenderScene(deltaTimeSeconds);

	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);

	camera2->Set(glm::vec3(0, 100.0f, 0), camera->GetTargetPosition(), glm::vec3(0, 0, 1));
	RenderSceneOrtho(deltaTimeSeconds);
}


void Tema2::FrameEnd()
{

}


void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, bool proj, const glm::mat4& modelMatrix, const glm::vec3& color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// Render an object using the specified shader and the specified position
	glUseProgram(shader->program);
	
	//trimit culoarea
	glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));
	glm::vec3 pos = camera->GetTargetPosition();
	//trimit pozitia masinii la vertex pt a calcula curbura
	glUniform3fv(glGetUniformLocation(shader->program, "car_pos"), 1, glm::value_ptr(glm::vec3(pos.x, pos.y - 0.675f, pos.z)));

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = proj ? camera->GetViewMatrix() : camera2->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(proj ? projectionMatrix : projectionMatrixOrtho));

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->m_VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	float cameraSpeed = 2.0f;
	float sensivityOX = 0.001f;
	float sensivityOY = 0.001f;

	if (window->KeyHold(GLFW_KEY_W)) {
		glm::vec3 pos = camera->GetTargetPosition();
		//fiecare sfera de coliziune a masinii are raza de 0.3
		if (distance(pos, firstCarPos) <= 0.6f || distance(pos, secCarPos) <= 0.6f || distance(pos, thCarPos) <= 0.6f) {
			return;
		}
		else {
			camera->TranslateForward(deltaTime * cameraSpeed * 2);
			pos = camera->GetTargetPosition();
			bool inside = false;
			//verific daca translatia facuta ma scoate afara de pe pista
			//daca, da atunci dau undo la translatie si astept o alta comanda
			for (int i = 0, j = 0; i < ext.size() - 1 && j < in.size() - 1; i++, j++) {
				glm::vec3 A = ext[i];
				glm::vec3 B = in[j];
				glm::vec3 C = in[j + 1];
				float triangleArea = TriangleArea(A, B, C);
				float areaSum = TriangleArea(A, B, pos) + TriangleArea(A, C, pos) + TriangleArea(B, C, pos);
				inside = inside || (triangleArea >= areaSum - 0.01f && triangleArea <= areaSum + 0.01f);

				B = ext[i + 1];
				triangleArea = TriangleArea(A, B, C);
				areaSum = TriangleArea(A, B, pos) + TriangleArea(A, C, pos) + TriangleArea(B, C, pos);
				inside = inside || (triangleArea >= areaSum - 0.01f && triangleArea <= areaSum + 0.01f);
				if (inside) {
					break;
				}
			}
			if (!inside) {
				camera->TranslateForward(-deltaTime * cameraSpeed * 2);
			}
		}
	}

	if (window->KeyHold(GLFW_KEY_A)) {
		glm::vec3 pos = camera->GetTargetPosition();
		if (distance(pos, firstCarPos) <= 0.6f || distance(pos, secCarPos) <= 0.6f || distance(pos, thCarPos) <= 0.6f) {
			return;
		}
		camera->RotateThirdPerson_OY(cameraSpeed * deltaTime);
		angle -= cameraSpeed * deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_S)) {
		glm::vec3 pos = camera->GetTargetPosition();
		if (distance(pos, firstCarPos) <= 0.6f || distance(pos, secCarPos) <= 0.6f || distance(pos, thCarPos) <= 0.6f) {
			return;
		}
		else {
			camera->TranslateForward(-deltaTime * cameraSpeed * 2);
			pos = camera->GetTargetPosition();
			bool inside = false;
			for (int i = 0, j = 0; i < ext.size() - 1 && j < in.size() - 1; i++, j++) {
				glm::vec3 A = ext[i];
				glm::vec3 B = in[j];
				glm::vec3 C = in[j + 1];
				float triangleArea = TriangleArea(A, B, C);
				float areaSum = TriangleArea(A, B, pos) + TriangleArea(A, C, pos) + TriangleArea(B, C, pos);
				inside = inside || (triangleArea >= areaSum - 0.01f && triangleArea <= areaSum + 0.01f);

				B = ext[i + 1];
				triangleArea = TriangleArea(A, B, C);
				areaSum = TriangleArea(A, B, pos) + TriangleArea(A, C, pos) + TriangleArea(B, C, pos);
				inside = inside || (triangleArea >= areaSum - 0.01f && triangleArea <= areaSum + 0.01f);
				if (inside) {
					break;
				}
			}
			if (!inside) {
				camera->TranslateForward(deltaTime * cameraSpeed * 2);
			}
		}
	}

	if (window->KeyHold(GLFW_KEY_D)) {
		glm::vec3 pos = camera->GetTargetPosition();
		if (distance(pos, firstCarPos) <= 0.6f || distance(pos, secCarPos) <= 0.6f || distance(pos, thCarPos) <= 0.6f) {
			return;
		}
		camera->RotateThirdPerson_OY(-cameraSpeed * deltaTime);
		angle += cameraSpeed * deltaTime;
	}

	if (window->KeyHold(GLFW_KEY_Q)) {
		camera->TranslateUpward(-deltaTime * cameraSpeed);
	}

	if (window->KeyHold(GLFW_KEY_E)) {
		camera->TranslateUpward(deltaTime * cameraSpeed);
	}
}


void Tema2::OnKeyPress(int key, int mods)
{
}


void Tema2::OnKeyRelease(int key, int mods)
{

}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// Add mouse move event
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		if (window->GetSpecialKeyState() == 0) {
			camera->RotateFirstPerson_OX(-sensivityOX * deltaY);
			camera->RotateFirstPerson_OY(-sensivityOY * deltaX);

		}

		if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
			camera->RotateThirdPerson_OX(-sensivityOX * deltaY);
			camera->RotateThirdPerson_OY(-sensivityOY * deltaX);
		}
	}
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
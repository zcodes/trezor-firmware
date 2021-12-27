#ifndef MINA_POSEIDON_PARAMS_3_H_
#define MINA_POSEIDON_PARAMS_3_H_
#include "crypto.h"

//
// Poseidon_3 hash function parameters
//

#define ROUND_COUNT_3  54
#define SPONGE_WIDTH_3 3
#define SPONGE_RATE_3  2
#define SBOX_ALPHA_3   7

// Round constants
static const Field round_keys_3[ROUND_COUNT_3][SPONGE_WIDTH_3] =
{
    {
        {0xa7eaec68b00f442f, 0x6f59e1a835643145, 0x5e1085dc39694ee7, 0x31f43b11041ce57e},
        {0xaf9d994fe02cad85, 0xd023d4dba24251f2, 0xab2b289011b10b15, 0x136703f8461e5900},
        {0x17d4dcf1505a5b8e, 0x905301ba46f35eda, 0xbf183c861c890269, 0xcd2255c8e8bfad7}
    },
    {
        {0x652519f35396e7b9, 0xf939da87dd2565f7, 0x77863388eeb1739a, 0x2ef0165a2c2e5844},
        {0x477f697921c5a46f, 0x2f5597aaf22a3c0e, 0xea51d9a2b20ef8ee, 0x27f19ea80ac259a1},
        {0x475253132eafc552, 0x691f153f119e2158, 0x3463b3d75dc73170, 0x3875a2611a0025e4}
    },
    {
        {0xc1500848eb7005a6, 0x61e2acd1faf26ff8, 0xf50c67341380fa2f, 0x14e95cd68d6778e},
        {0xa82b34cad0ebd99e, 0x771d54af176024ba, 0xa343788ac43bbde2, 0x31c9ab354d21b72a},
        {0x3deb92a1d0140aab, 0xb710f83bb2ce1349, 0xf3dc159cb9171a97, 0x6c3eac66a23c368}
    },
    {
        {0xd25cd659b62b584, 0x3a52d26b8092be14, 0xe36c5155d1e135b, 0x16deeee5467c7a70},
        {0x9b446f453cb50aca, 0xeadf51c8f0af7a58, 0x75da6f44c4a04ca2, 0xccc39a8549cb487},
        {0xe09191801b0fdabc, 0xa7452c89066d4bd0, 0x5244883ddc1fd9cf, 0x3862cc172ab37d2f}
    },
    {
        {0x6a87a9921ce9aabf, 0x5d701ed9a030bc04, 0xf99642aa2a835ec8, 0x28319fcffc61a144},
        {0xafb8a54d87a15b37, 0x31e93428debef477, 0xd328f47b48852d58, 0x1e9fb7a3f8c8e39f},
        {0xa58f9aa995fcab1, 0x5fb0135046864286, 0x6ad292753e09ec37, 0x24cb4d2ff2e77907}
    },
    {
        {0xbebb7c8e57280d90, 0xa2575331fa3f83b, 0x1bdaa15abc6032e4, 0x137f4d26f7fc2b89},
        {0x8ea04d93f2a3faba, 0x76d83e0976143e11, 0xde45171e6c479b69, 0x3ec500ad89afdcfe},
        {0x9741b7a395379855, 0x916b939a142e1e68, 0x4fda77496b815109, 0x19d993fa47fbc1a7}
    },
    {
        {0x31d6e313b491fe95, 0x19f07f1a489cb20a, 0x517ac8eb07d91f83, 0x16bd2c0e204bb5dd},
        {0x42170b694f7b1926, 0xdde594e9f6060540, 0xbdb7c66abef575a5, 0x26c2c710d9aea3c9},
        {0xccfb5e566332e76d, 0x4e73df588d423339, 0xa4ede3fda7178916, 0x28fc0255accddbf0}
    },
    {
        {0x2fc2f7a5cf4eeb76, 0x709a30d94daa56c0, 0x67a5dd9c696533dc, 0x3ace056f839007b7},
        {0x7769ae0082cae71b, 0xdaa4d4c08c74011a, 0x919a01423e32424d, 0x22467d425c99d5c9},
        {0x6bcd3613ace4871c, 0xb0bc217531069def, 0x55348199fa2b487b, 0x2385b602228e77ee}
    },
    {
        {0x7022e34179f7fd32, 0xd7d122a3a91838e2, 0x20e0714f41741103, 0x55ca30203c35d65},
        {0xeac64a4e27a532ac, 0xfad717669ae09d08, 0x16088f7d30d724f1, 0x37bb5a0c062e6400},
        {0x7c34a314bcba4635, 0xbfdd1e4cda8ce53, 0x7b9809bd9828ebbb, 0xbe17455fb915f02}
    },
    {
        {0x3eedf4d1439104cc, 0x6944849f8d44d187, 0x4dbe80e8f415dfdc, 0x1df9173c6d047e75},
        {0xbe0b090e46076c2e, 0xdc964a5825b49df4, 0xc8c690008b31f4cc, 0x2a7eea60cbe9e9c2},
        {0x7e3ea7d6debb93ed, 0x697818afff9c8ff8, 0xbd716ed0d057327c, 0xb4a53cd8838ca03}
    },
    {
        {0x9249570785f595ac, 0xa8baedefd054d755, 0x4e349f30983cb2d1, 0x20e038ae5219f06f},
        {0x1e6b0d84b6b0bd2b, 0x28036e399a182d83, 0x9b4db5cdc8f5900f, 0xba7d843e40009},
        {0xc1862aa768e1a9de, 0x134ee89ea258d84b, 0xa344c6a98af5c8e1, 0x35868f24584f89c0}
    },
    {
        {0x6562561d6d2bd2eb, 0xe906eab7bfb25ce1, 0xdc3286360790ca24, 0x95cf9864fa2e99f},
        {0x8df04360bfee0f49, 0xf36da4b0797f28ab, 0x973b5df087ce6868, 0x31b4ebf5790f547a},
        {0x5236c69070d6a6cd, 0xe3483908bed7cd3a, 0x230cf521e8f636d5, 0x1369fed0b1ebdd4e}
    },
    {
        {0x6b84b4d8a249ce56, 0xf8f9e37aee88a381, 0x547e20f39ea3ee67, 0x1a521535f4c4609a},
        {0xece3e35b95cf0b40, 0xc3f355a2343a9bba, 0x352dca283e1dc20, 0x18533c495c159ea5},
        {0x5c080eb0e667f655, 0xae7eedb84aaa82cf, 0xc40800efbbfb36e2, 0x23f8f08e412d848e}
    },
    {
        {0xe47de2b666816c07, 0xfd084e96b59c227a, 0xffee4cb4adefb5b9, 0x27873596008bf329},
        {0x74c07063640d932f, 0xdcafa7d16923f8c6, 0x69cc607697b5fa58, 0x3fe8dcd350f158a0},
        {0x4685e7e0534a4487, 0x3e0a5a82c1308413, 0xe8f4882500745f38, 0x15d7152e439e3e39}
    },
    {
        {0x1b6a574d8750bc73, 0x106eecce46abf1e6, 0xaac26d11ddaa2fa6, 0x38e31bc0f50b77a8},
        {0x46f4e9b5d03cc39d, 0xcc3f83cbed376830, 0x9b08a2cd1eb3c25c, 0x1b205334b2958429},
        {0xd58f58d5f1219de9, 0x1cfaa7547a262198, 0xcded38f37a2a880d, 0x177392b21a898be2}
    },
    {
        {0xabc2d6caf40fda5a, 0xeb350f697f26ad45, 0x3a8945142b944356, 0x646f6d15a42f200},
        {0xff43256a693e8ab0, 0x2d305df7c836dcae, 0xccde78bd165aba3, 0x784cf8c1e09fd03},
        {0x11c0a75dfb1d6922, 0xa7dc5f890e8b9385, 0xaf784b2ec0758e11, 0xb79376b50d40562}
    },
    {
        {0x4cfa43b93f239b43, 0x8465e6265d898268, 0x1288bcdb6fff50f4, 0x819462af6ba8c78},
        {0xba07b8ffdba328b0, 0x4215ac492cce28b3, 0x9730ec3e2f2188f3, 0x10bc9871d3004f71},
        {0x5a31626caca7283f, 0x40424e694cc63e9a, 0xcceb1adc2a52c6b9, 0x2836d9e7ccb5b686}
    },
    {
        {0xf2191ab8935e3139, 0x6263e421438839a9, 0xf36d5611a925964c, 0x3ad254981c5584f},
        {0x77f3cbe4b7ec0506, 0x14cfd27cad91e7f6, 0xd66382bc65d30343, 0x11eb1499a7eb9296},
        {0x8306372b9fccfce6, 0xff3f90b389214f3f, 0x589837150e5e7261, 0x330673584db884bd}
    },
    {
        {0xbf438550dc08ca8c, 0xa67d5b92e6b2d9f4, 0x45f1b4851f11355e, 0x3197089756209bf0},
        {0xa7bb2e56a3383876, 0xa1b85198d1a992d9, 0x807a2bb7e34327c6, 0x1a7be86ebc9ceb25},
        {0xb67db6d33a48b0df, 0x20bcebe6769fcc54, 0x2c5defebf8a107df, 0x3c0e75597f21e444}
    },
    {
        {0x3ee09fdbfc3969f8, 0xfa1d3b39ae852fe9, 0xf9a75fb275dc4bf3, 0x2a38ad56c662e4c3},
        {0x436111724d967633, 0x7bc3452ae208f145, 0xfa8b9c79a56e7177, 0x3328b53b61a96c1a},
        {0x7dc685041c4a8de8, 0xcc8d3bf1000d5962, 0x53f0f1b8456b9659, 0xe5633a4ee33b43f}
    },
    {
        {0x50e48fb72f31229f, 0xa5b753a3355553ad, 0x1c81ccd682c6dbf0, 0x6c795d332f94020},
        {0xf0fdbce25dad3d31, 0x9c588e128b3cebf0, 0x278767bad1a401ec, 0x12fd7107dacf168b},
        {0x98f200455814ae5c, 0x47f8009a5ae445bd, 0x487393e3ebc8077f, 0x3d579e85ebce0cba}
    },
    {
        {0xe8df711446a2a238, 0xde4cc56b9510c04c, 0xb69991b66d096631, 0x1ab962f474f31f94},
        {0x994397076df00b58, 0x655b22de2fc1d376, 0x8c4152fcacaf0b18, 0x20051bcd8c1c3f37},
        {0xd66fe84e8d8d5bcb, 0xba022f54bb73200, 0xdaf17b9ad85c1d89, 0xd939c085418afd2}
    },
    {
        {0xd5e63b74c211bc, 0xb12fc9c1def2e171, 0x44eaf0fd6faad3ea, 0x242c8eaf5e3f8025},
        {0xbf97706088f0ff2e, 0x1d8dad8a65750bd0, 0xc29b958d06fc399f, 0x134e81757cb8421e},
        {0x90be439afe7faf2, 0xc4b4762c8cc16bea, 0x1e477be4c4f40c9b, 0x23b4d50dc7ec40d7}
    },
    {
        {0x333f64ebf7d2e7a8, 0x870b1f5dbdb11bdf, 0x74c9542dd65f4221, 0x128bb297ac5b8087},
        {0x74cdc264c1c32632, 0xcdbd7f0678ca09f9, 0xeb378a38bded711e, 0x1c4c18ced367bb10},
        {0x436cf961456330a9, 0x5261fdb10401ba02, 0x5aa07cfe8724969b, 0x17d4aa22d7539f30}
    },
    {
        {0x22577a304a9a99c0, 0x26990766bceeaf3e, 0xf44a18d2965b75e6, 0x2b9ccb511447556b},
        {0xb4816bee4c57ea14, 0x657c7ff22284eddf, 0x80327f4b561a57a, 0x185d288b2163e408},
        {0x2521ae71bb3e92cd, 0xcfa6a3908f9422a4, 0xc5566b5d0cba3b6a, 0x19186ee34b32adf1}
    },
    {
        {0x6ababe0d83a14804, 0x48a2976613fe9c87, 0x8fd28b1e6637a35f, 0x36be57a5b7dc101},
        {0x9409b310f8dedbd5, 0xdcb6ff2dd7ade6e8, 0x58ef424bfb1e96cf, 0x59c25a9e8435caa},
        {0x6017336865718144, 0x3b735147994d3174, 0x556dacb8916ce5fb, 0x6db68d040cf3894}
    },
    {
        {0xfb28daa548eae196, 0xdee9f1fb48ad3a18, 0x8aeb7ea74f7458e0, 0x2f9a904f8eca76f4},
        {0x78ab93c53d27c0a9, 0x7cbd38a19659d91e, 0x6190fc4c6fcea287, 0x392493ffebdcf4db},
        {0xc288a18be7c7b29a, 0x293635495be4702a, 0x8eabeaa050c4e742, 0x3ee20e43c9a7e3a9}
    },
    {
        {0xb247c20bce94d2fb, 0x7b37f102aba6dc41, 0x4e4cd6f2dfd06908, 0x2a4b7450c10e8656},
        {0xb8530701f1190812, 0x8852c8a7dd78c157, 0xef99612848a09ee6, 0x18cac72301419e17},
        {0x3bf72719696125f9, 0xa9153cf6b92ed66c, 0x5dee1004fa68e8ad, 0x2b0ab9d9c7146b8d}
    },
    {
        {0x536e86c644632835, 0xfb528949c3d10378, 0x93cef66ee69cc174, 0xef7cc63c029b540},
        {0x3f71db10b88e45e1, 0x6a1cdfba155d75ba, 0x3ae6893f9c00830, 0x1f4aa1edaeea25af},
        {0x6c911d68b8bae9c7, 0xb562eb9132f9adcf, 0xb99552efd2922e09, 0x1aee619f6e53e5d1}
    },
    {
        {0xc3e7eba538b57ce4, 0xe31fc856b301a0ed, 0x907bed9fd07a7f39, 0x2d8b156182efe6c2},
        {0x3c46848a7286379b, 0x8c1ce4abf2bdac01, 0xfe59adadfa57ab2d, 0x118d842df28ba098},
        {0x40346673db347d50, 0x9e9e286fbc221bac, 0x629f295d0d56a062, 0x2c7473af62ed3627}
    },
    {
        {0x752c0e54b9084c5e, 0x9876503df35123f2, 0x4033dd4b36b302eb, 0x1f2f3ed1fdf6db7b},
        {0xc1aac2892f6ad5a0, 0xa4d2c62a12460d75, 0xfdfc27280cba16d8, 0x32e7b499b5afa89f},
        {0xa93b9c721205117, 0x29f32892dd66377a, 0x5db34e58446659b5, 0x35b9a957235ce56c}
    },
    {
        {0x27dd8772ea8dd6fb, 0x7657580893377a80, 0x98116540d5d734fc, 0x1784ef3dbedaa7f1},
        {0x7ea4b4453558ad1e, 0xe79ae8a84479841f, 0x1448d360ba9dceaf, 0x29004bf2b27c1cc},
        {0x9be743f9caccf63, 0xf852240a936f7e9e, 0xb1cd6029e13842dd, 0x255bdfce8905f3d5}
    },
    {
        {0x7415ee9f67631e70, 0x545553ffaf7d2592, 0xde51ae9194c9a7b2, 0x304409b176b5751d},
        {0x9c447ba5504d7b34, 0xfb288ee73506db89, 0x4cb32cefbc235f79, 0x3f2084c1c1605185},
        {0xdd24109730f8173c, 0x58b00f316d07a773, 0x1e86bdb19d121999, 0x3c46e8c04c88a74f}
    },
    {
        {0x9ff193f33610856, 0x7cff32dc29fafc7f, 0xab1176764a90b2b5, 0x37a314e9261a12bc},
        {0xea6753fdcff09509, 0x4222dd5e1a66381d, 0x6056412c14d4667e, 0x30f19d0141776609},
        {0x383a756cdd6bbf4a, 0x4b314310043c1039, 0x7aa6b018a79ad7a4, 0x2d78d62918aa8e15}
    },
    {
        {0x2c202fafd61e9da3, 0x89be41afc9c1f1fe, 0x15b6d718ea74a2d, 0x1c1c7eb84e510588},
        {0x342bbc7354bde7a5, 0xc995adfbe2005f6b, 0x85a618fbe9ccea3c, 0x3d61c03132b831b7},
        {0x84f0053a596c462a, 0x3725a512ef2e7da, 0x9866bda3e8d65025, 0x16307f53c31b9b2d}
    },
    {
        {0xe11a11d7ad7c2941, 0xa196efe53885dac4, 0x401c98e5702a4b4d, 0x2627aedf841d2535},
        {0x6c137315f42a3d7, 0x1b27880c4b80acc2, 0xd2463e7af19c63f8, 0x1ac361c3ae9360d0},
        {0x9ca7acb3c1e8764c, 0xa3abf95002b63ec7, 0x607aec3d66927bd, 0x350322d135e4b5b0}
    },
    {
        {0x8767a56f58823fb1, 0xa17115bf335777f7, 0x2d2d011c09bc4b6, 0x23f5c30c8bec1802},
        {0x469e5aee1e354cff, 0x19b662c5ee659a15, 0x8222b7d81ef2e3d0, 0x25dbe21f1dc14bbb},
        {0xcf3036e9bef51413, 0x2ec597011f15b75d, 0x1b6f15cf41987ffb, 0x243e4331eaa8ca3f}
    },
    {
        {0x7e12d7264aa9d64a, 0x986866249a40df19, 0x224d9731f988d510, 0x3da1029b04d0699},
        {0x63ce9c82f26c2122, 0xd07b42737417e607, 0xdffe6c18b3e32c4e, 0xcb558a55e33635b},
        {0xa9e72e1bbb1e1ac6, 0x7c0eac490aa84fe, 0xc716b352d91ef297, 0x17c0df062139a3b0}
    },
    {
        {0xb6e0e5acafd75f1b, 0xe79ccc9d178be022, 0xa62fb84a4594821, 0x118ff23cec0e9ab7},
        {0xdaeb6bb9649cc9ed, 0x2b1c328f5bb8662f, 0x5512487ef834c9ff, 0x10a6d8b760d8381f},
        {0x55bcd8a0925661ee, 0x92289bf6cb46afe5, 0xa2b1c38630374b57, 0x1a382c83ffa7d479}
    },
    {
        {0x5b9ed41d83b22227, 0x861a12cf34e20309, 0x722187ecc9fb01cd, 0x3d7bb8d8fbbf19aa},
        {0x56b2351caaa77660, 0x5649f8afb77f8250, 0x179b134befc40c2a, 0x117b5ac7fab8a73b},
        {0xbb53d76efa39fb63, 0x3821ad8ab6648b10, 0x860e686568c98df, 0x181510039e8bf016}
    },
    {
        {0xabcab07505b93cf, 0x4088c208ddc38a79, 0x60aa91fcf3d30a95, 0x1cca89ba50887f72},
        {0x756ea77ac464fb2f, 0xb54ae17c6bba044a, 0x3350ae0df3e90873, 0x186b9034baedae26},
        {0x813cc8de3ccd6501, 0x612bffa93d7579f9, 0x5ad42ea9bc5bbd6c, 0x123ed625db167d32}
    },
    {
        {0x2db34a3c43a01e76, 0x93bf90bd39919618, 0xb62aafc5a062e6ec, 0x30e33d44bc2851c0},
        {0x74dcd4e07d98a23b, 0xb390d88ce8c1c12d, 0xc60d4855592f4f12, 0x3d3808ea888bc3ef},
        {0x6b6125132d3b6c9b, 0xefb93b37d8c98901, 0x5396ab9879f061f3, 0x26814970d99a4859}
    },
    {
        {0x65a39dff4c093ac6, 0xdb8b03741b7d38b3, 0x34e5285eb2732099, 0xcffcab13b9c402},
        {0xba151ecf346e77e5, 0xdc944d4cedf21de6, 0xbf6157bdc8bf2df3, 0x39a726986b4d3042},
        {0x44150981f7bb214f, 0xdaf82667aa9df080, 0x4cb0e4251cbd837c, 0x96407a7f4c8919d}
    },
    {
        {0xab327a1874a4376d, 0x2ac8b215b83ec21, 0x72ccc1310e756e19, 0x337f64158d29681d},
        {0xfdd547b12cd1c47c, 0xeda570d77efb35ef, 0xee673a9c5c5c6c24, 0x1069938e1c2d5fb8},
        {0xe18e30b72b41cebd, 0x9530ddc35a81e36e, 0xcb076c0372dd1f10, 0x1a6810a2f1aee9a3}
    },
    {
        {0xf42b4184e45ef9b0, 0x621c9c9f4b7805bb, 0xe64a4966c57c625e, 0x331dfaac4fca1afe},
        {0x5745585e5aa8e18, 0x48138c500616daa7, 0x740880dd14d57c7b, 0x621e4237f79bc67},
        {0x7752004d293d5b5d, 0x32176da95d737cf5, 0xe14bb9ef4281cf03, 0x56e679688dec9b0}
    },
    {
        {0xcf96c57a6c6dced2, 0x9d0641008bc1e8e6, 0xaf7c8a61f00aea43, 0x3de7cc921f006f35},
        {0xe5a284274906c87c, 0x3f57a5c227ad9ef7, 0x8ca3d4e109a701b9, 0x23fa54fa0717813},
        {0xa418bb283bcf6e4c, 0xfe2037c5295b505d, 0xcaa0956e946f4a29, 0x2a042a0cc94e6eb7}
    },
    {
        {0xd8df14778b795fec, 0x16c27f664e2ea362, 0xe24b2f6edd5eaebe, 0x14c903e18d6d1fc3},
        {0x89586ac5b4450805, 0xadc6ee91f14ae921, 0xc6bc4c3b0f873a03, 0x351fccf49d14543a},
        {0xd3601422d37aaa9e, 0x988c203077dd68e9, 0x1cae7a1d3150e958, 0x1292d11437a1acd4}
    },
    {
        {0xe53e955d71efa6c, 0x8fe1b41936d8c2e, 0xc1115be6796700ba, 0x16f2577444f333ac},
        {0xb46fdc6c76b91715, 0xba7c98748d26f41a, 0x8a767b731b64d9a2, 0x380f78e27ff89d8f},
        {0xfd9976c4488e5f16, 0xbeaf3cb1cf0cfc03, 0xf2bc02883a339b2a, 0x3a669a9fec53277f}
    },
    {
        {0x7489c5648c85dee1, 0x7f9d020bd71f0bc3, 0xcf347020818fd255, 0x34ce3d2873ef623b},
        {0x82891abb666f3ff4, 0x370d13492155070d, 0x26c0426048d99ec2, 0x89cceef0d956cc},
        {0xf161875b3c921a95, 0xe0ee4bd7518d961a, 0xcd3614b6acaf6d93, 0x27ef7c723667b755}
    },
    {
        {0x13fb2dcf933fd4d, 0x103f7deb9f64a5aa, 0x26d375314a7a8189, 0x1fb433f9d0d1a1af},
        {0xa49a8d2583ef3bdd, 0x9a7a627ea2417df1, 0x93277db6c1298ce4, 0x224efdd22b53fe03},
        {0xd9ef2ab6918b48ed, 0xab7f9a26705488c0, 0xc179467a22565381, 0x3c8192acf2659bbf}
    },
    {
        {0x90c3c01685741695, 0x8fc7d42cbdf62efb, 0x8ca74bdebec3a42b, 0x2d07192135436805},
        {0x597f91232510722, 0xb6ff30a1505c4ea8, 0xdd22fd456a942afe, 0x1cb7fdcfaa9bdd05},
        {0xdcf520384f1ddaf0, 0xeb0ee12c33953a63, 0x9a3a10c7e3aa6f41, 0x1471506df8d986b4}
    },
    {
        {0xcd605e2791e6a5e5, 0x1c27ce94b83f068f, 0xb172e53c549a3c12, 0x21f264ce80eef855},
        {0xfe4a5caf09cd178f, 0xd76b6393a8b8597f, 0x828a31309599f1fa, 0x1d7eb56ca9c920f6},
        {0x589bdbc5eb3cc1fc, 0xa5ab6823129a3ed3, 0x1f5b9bd5aca5d3ec, 0x64aae35f2e4b6bc}
    },
    {
        {0x365002f75230793, 0x2b8d834bd6a4c78f, 0x7f445a46d203a93d, 0x265c579246ab3106},
        {0x4983fb4db3ca5acd, 0x770c07654eeefaab, 0x54456b57fa1fd5af, 0xb92cdbcf718c353},
        {0x1ac57ac76f928fe, 0x816c4d52b670680, 0x39b8598cba83c07d, 0x29af1b8a7d9ed796}
    },
    {
        {0x715eb17f6f71404f, 0xd081dca0cad01695, 0xdeefdf16929d4947, 0x3d7f767198a2e29b},
        {0xebfb13feb0205761, 0x73e014ef38a3d8cb, 0x113bd31d16c16db7, 0x288eacb7eaa4d63c},
        {0x5d22751f1f2c937d, 0x50cc30867fe04f68, 0x70917ec415d4ac2c, 0x58c8b7c2b87e78b}
    }
};

// MDS matrix
static const Field mds_matrix_3[SPONGE_WIDTH_3][SPONGE_WIDTH_3] =
{
    {
        {0x19ebb0733ab608d5, 0xba29ddd056f8255c, 0x90b26832e301952d, 0xd393e38c7a5d0ab},
        {0x1370915e16c94656, 0xa9d4f14bbd2ea831, 0xbe629fe93a27e612, 0x21571a7cc32e18af},
        {0xb34a71b4f799ea19, 0x9685275173ff9b6b, 0xc2aa354b7f11d698, 0x15691c24c0a9f088}
    },
    {
        {0xae932a9d486aface, 0xbef0293e7653db35, 0x279408c4d244c0d1, 0x39dd8040ea0c8d80},
        {0x9d32fbdbaf9bbe27, 0x999e1c5168806efa, 0x7d5f270b3cb077a8, 0xb4ac0738805f8de},
        {0xcfc1e61e0bcf9d5f, 0x12346d0d47576a9, 0xdbd1b0da710c7b5, 0x1db48905ceecc479}
    },
    {
        {0x89dde65da2c627b, 0x873534c4aaf1243a, 0xa4f76bf3e5b626c5, 0x22a75f7cd3cad9d3},
        {0xb58ea56557361c3b, 0xd6cbb2cb0bc99acb, 0x5fee1fb5b71b86dd, 0x11d87bbca8e20fbe},
        {0x1d511fe638baa5c8, 0x7b6c7932776e1032, 0x45b27ee070531360, 0x19eb7c902ac90c5}
    }
};

// initial sponge state testnet
static const Field testnet_iv_3[SPONGE_WIDTH_3] =
{
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};

// Initial sponge state mainnet
static const Field mainnet_iv_3[SPONGE_WIDTH_3] =
{
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};
#endif

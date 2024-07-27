#ifdef FASESBOX_INCLUDES_test
	else if (!strcmp(basename(argv[0]), "[")) return test_main(argc, argv);
#endif
	else if (!strcmp(basename(argv[0]), "box") && argc == 1) {
		printf("fases box.\n");
		printf("Usage: box command [args] ...\n");

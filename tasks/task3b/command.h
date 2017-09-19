/*
 * Initialise the command object.
 */
void initCom(void);

/*
 * Write the command into a shared memory variable.
 */
void writeCom(unsigned char command);

/*
 * Read the command from a shared variable.
 */
unsigned char readCom(void);

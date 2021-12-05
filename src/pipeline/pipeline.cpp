#include "stdafx.h"
#include "pipeline.h"
#include "pipeline_imp.h"
#include "worker.h"

void* pipeline_create()
{
	return new pipeline_imp();
}

void pipeline_delete(void* pipeline)
{
	delete (pipeline_imp*)pipeline;
}

void pipeline_start_async(void* pipeline, output_func output)
{
	return ((pipeline_imp*)pipeline)->start_async(output);
}

void pipeline_stop_async(void* pipeline)
{
	return ((pipeline_imp*)pipeline)->stop_async();
}

void pipeline_add_procedure(void* pipeline, procedure_func proc)
{
	return ((pipeline_imp*)pipeline)->add_procedure(proc);
}

void pipeline_wait_for_idle(void* pipeline)
{
	return ((pipeline_imp*)pipeline)->wait_for_idle();
}

void pipeline_set_file(void* pipeline, const char* file)
{
	return ((pipeline_imp*)pipeline)->set_file(file);
}

void pipeline_reset_file(void* pipeline)
{
	return ((pipeline_imp*)pipeline)->reset_file();
}
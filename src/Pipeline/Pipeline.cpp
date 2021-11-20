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

void pipeline_start(void* pipeline, output_func output)
{
	return ((pipeline_imp*)pipeline)->start(output);
}

void pipeline_stop(void* pipeline)
{
	return ((pipeline_imp*)pipeline)->stop();
}

void pipeline_add_procedure(void* pipeline, procedure_func proc)
{
	return ((pipeline_imp*)pipeline)->add_procedure(proc);
}

void pipeline_wait_for_idle(void* pipeline)
{
	return ((pipeline_imp*)pipeline)->wait_for_idle();
}
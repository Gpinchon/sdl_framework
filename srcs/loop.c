/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpinchon <gpinchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/06 18:59:58 by gpinchon          #+#    #+#             */
/*   Updated: 2016/12/20 15:44:17 by gpinchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <internal_framework.h>

Uint8	mousemoved(void *framework)
{
	return (
	((t_framework*)framework)->lastmousepos.x != get_mouse_pos(framework).x
	|| ((t_framework*)framework)->lastmousepos.y != get_mouse_pos(framework).y);
}

void	check_mouse(t_framework *f, SDL_Event *e)
{
	Uint8		type;

	if (mousemoved(f) && f->mousemove[e->button.windowID].function)
		f->mousemove[e->button.windowID].
			function(f->mousemove[e->button.windowID].arg, get_mouse_pos(f));
	f->lastmousepos = get_mouse_pos(f);
	if (e->type == SDL_MOUSEBUTTONDOWN
	|| e->type == SDL_MOUSEBUTTONUP)
	{
		type = e->type == SDL_MOUSEBUTTONDOWN;
		f->buttons[e->button.button] = type;
		if (f->mouse[e->button.windowID][e->button.button][type].function)
			f->mouse[e->button.windowID][e->button.button][type].
				function(f->mouse[e->button.windowID][e->button.button]
					[type].arg, e->button.button);
	}
}

void	check_keyboard(t_framework *f, SDL_Event *e)
{
	if (e->type == SDL_KEYDOWN)
	{
		f->keys[e->key.keysym.scancode] = 1;
		if (f->keydown[e->key.windowID][e->key.keysym.scancode][e->key.repeat].
			function)
			f->keydown[e->key.windowID][e->key.keysym.scancode][e->key.repeat].
				function(f->keydown[e->key.windowID][e->key.keysym.scancode]
					[e->key.repeat].arg, e->key.keysym.scancode);
	}
	else if (e->type == SDL_KEYUP)
	{
		f->keys[e->key.keysym.scancode] = 0;
		if (f->keyup[e->key.windowID][e->key.keysym.scancode].function)
			f->keyup[e->key.windowID][e->key.keysym.scancode].
				function(f->keyup[e->key.windowID][e->key.keysym.scancode].arg,
					e->key.keysym.scancode);
	}
}

void	framework_loop_once(void *framework)
{
	t_framework	*f;
	SDL_Event	e;

	f = framework;
	FRAMEWORK_DEBUG(!framework,
		NULL_FRAMEWORK_POINTER, "framework_loop_once");
	SDL_PollEvent(&e);
	if (e.type == SDL_APP_LOWMEMORY && f->low_mem.function)
		f->low_mem.function(f->low_mem.arg);
	if (f->loop.function)
		f->loop.function(f->loop.arg);
	while (SDL_PollEvent(&e) && !f->done)
	{
		f->done = e.type == SDL_QUIT;
		if (f->done)
		{
			if (f->onexit.function)
				f->onexit.function(f->onexit.arg);
			destroy_framework(framework);
			break ;
		}
		check_mouse(framework, &e);
		check_keyboard(framework, &e);
	}
}

void	framework_loop(void *framework)
{
	SDL_Event	e;
	t_framework	*f;

	FRAMEWORK_DEBUG(!framework, NULL_FRAMEWORK_POINTER, "framework_loop");
	f = framework;
	while (!f->done)
	{
		if (e.type == SDL_APP_LOWMEMORY && f->low_mem.function)
			f->low_mem.function(f->low_mem.arg);
		if (f->loop.function)
			f->loop.function(f->loop.arg);
		while (SDL_PollEvent(&e) && !f->done)
		{
			f->done = e.type == SDL_QUIT;
			if (f->done)
			{
				if (f->onexit.function)
					f->onexit.function(f->onexit.arg);
				break ;
			}
			check_mouse(framework, &e);
			check_keyboard(framework, &e);
		}
	}
	destroy_framework(f);
}

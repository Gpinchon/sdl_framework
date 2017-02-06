/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_image.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpinchon <gpinchon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/06 22:18:47 by gpinchon          #+#    #+#             */
/*   Updated: 2017/02/06 21:54:27 by gpinchon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <internal_framework.h>

void	*load_image_file(void *framework, const char *path)
{
	GSTRUCT		*istruct;
	SDL_Surface	*bmp;

	istruct = new_object(sizeof(t_img));
	bmp = SDL_LoadBMP(path);
	FRAMEWORK_DEBUG(!bmp, SDL_ERROR, "load_image_file");
	if (!bmp)
	{
		free(istruct->data);
		free(istruct);
		return (NULL);
	}
	SDL_LockSurface(bmp);
	if (bmp->format->BitsPerPixel == 32)
	{
		SDL_FreeFormat(bmp->format);
		bmp->format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	}
	((t_img*)istruct->data)->sdl_surface = bmp;
	SDL_UnlockSurface(bmp);
	return (add_image(framework, istruct));
}
